
#include <string>

#include "service.h"
#include "rpc.h"


namespace liq {

    std::map<std::string, CommonService*> ServiceManager::services;
    std::map<std::string, CommonSkeleton*> ServiceManager::skeletons;


    void ServiceManager::load_cfg(ArduinoJson::JsonObject &cfg) {
        ArduinoJson::JsonObject &service_root = cfg["services"];
        ArduinoJson::JsonObject &stub_root = cfg["stubs"];

        // 创建桩
        for (auto it = stub_root.begin(); it != stub_root.end(); ++it) {
            const char *name = it->key;
            auto stub_cfg = it->value;
            char name_buf[MAX_NAME_LEN];
            printf("stub name:[%s],module:[%s]\n", name, (const char*)stub_cfg["module"]);

            // load pb
            snprintf(name_buf, MAX_NAME_LEN, "%s.pb.so", stub_cfg["module"]);
            Module *module = ModuleManager::Load(name_buf);

            // create stub
            snprintf(name_buf, MAX_NAME_LEN, "%s.stub.so", stub_cfg["module"]);
            module = ModuleManager::Load(name_buf);
            CommonService *service = module->create_service();
            CommonStub *stub = (CommonStub*)service;
            stub->set_rpc(RPCManager::create_rpc("self", (const char *)stub_cfg["remote"]));
            services[name] = service;
            // stub->onload(stub_cfg);
        }

        // 创建服务和skeleton
        for (auto it = service_root.begin(); it != service_root.end(); ++it) {
            const char *name = it->key;
            auto service_cfg = it->value;
            char name_buf[MAX_NAME_LEN];
            printf("service name:[%s],module:[%s]\n", name, (const char*)service_cfg["module"]);

            // load pb
            snprintf(name_buf, MAX_NAME_LEN, "%s.pb.so", service_cfg["module"]);
            Module *module = ModuleManager::Load(name_buf);

            // create service
            module = ModuleManager::Load(service_cfg["module"]);
            CommonService *service = module->create_service();
            services[name] = service;
            service->onload(service_cfg);

            // create skeleton
            snprintf(name_buf, MAX_NAME_LEN, "%s.skeleton.so", service_cfg["module"]);
            module = ModuleManager::Load(name_buf);
            CommonSkeleton *skeleton = (CommonSkeleton*)module->create_service();
            skeletons[name_buf] = skeleton;
            skeleton->set_backend(service);
        }

        // 设置依赖
        for (auto it = service_root.begin(); it != service_root.end(); ++it) {
            const char *name = it->key;
            auto service_cfg = it->value;
            auto service = services[name];

            ArduinoJson::JsonObject &dep_cfg = service_cfg["dep"];
            std::map<std::string, CommonService*> deps;
            for (auto it = dep_cfg.begin(); it != dep_cfg.end(); ++it) {
                deps[it->key] = services[(const char*)it->value];
            }
            service->oninit(service_cfg, deps);
        }
    }
}

