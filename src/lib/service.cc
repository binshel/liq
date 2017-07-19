
#include <string>

#include "liq/service.h"
#include "liq/rpc.h"

namespace liq {



    ServiceManager::ServiceManager(ArduinoJson::JsonObject &cfg) {
        add_stubs(cfg["stubs"]);
        add_services(cfg["services"]);
        add_accepts(cfg["accepts"]);
    }

    void ServiceManager::add_stubs(ArduinoJson::JsonObject &stub_root) {
        // 创建桩
        for (auto it = stub_root.begin(); it != stub_root.end(); ++it) {
            const char *name = it->key;
            auto stub_cfg = it->value;
            char name_buf[MAX_NAME_LEN];
            printf("stub name:[%s],module:[%s]\n", name, (const char*)stub_cfg["module"]);

            // load pb
            snprintf(name_buf, MAX_NAME_LEN, "%s.pb.so", (const char*)stub_cfg["module"]);
            Module *module = liq->module_manager->load(name_buf);

            // create stub
            snprintf(name_buf, MAX_NAME_LEN, "%s.stub.so", (const char*)stub_cfg["module"]);
            module = liq->module_manager->load(name_buf);
            CommonService *service = module->create_service();
            CommonStub *stub = (CommonStub*)service;
            stub->set_rpc(liq->rpc_manager->create_rpc(RPCManager::IPC_SHM, name, (const char *)stub_cfg["remote"]));
            services[name] = service;
        }
    }

    void ServiceManager::add_services(ArduinoJson::JsonObject &service_root) {
        // 创建服务和skeleton
        for (auto it = service_root.begin(); it != service_root.end(); ++it) {
            const char *name = it->key;
            auto service_cfg = it->value;
            char name_buf[MAX_NAME_LEN];
            printf("service name:[%s],module:[%s]\n", name, (const char*)service_cfg["module"]);

            // load pb
            snprintf(name_buf, MAX_NAME_LEN, "%s.pb.so", (const char*)service_cfg["module"]);
            Module *module = liq->module_manager->load(name_buf);

            // create service
            snprintf(name_buf, MAX_NAME_LEN, "%s.so", (const char*)service_cfg["module"]);
            module = liq->module_manager->load(name_buf);
            CommonService *service = module->create_service();
            services[name] = service;
            service->onload(service_cfg);

            // create skeleton
            snprintf(name_buf, MAX_NAME_LEN, "%s.skeleton.so", (const char*)service_cfg["module"]);
            module = liq->module_manager->load(name_buf);
            CommonSkeleton *skeleton = (CommonSkeleton*)module->create_service();
            skeletons[name] = skeleton;
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

    void ServiceManager::add_accepts(ArduinoJson::JsonArray &accept_root) {
        // 创建服务和skeleton
        for (auto it = accept_root.begin(); it != accept_root.end(); ++it) {
            printf("accept:[%s]\n", (const char*)(*it));
            liq->rpc_manager->create_ipc(RPCManager::IPC_SHM, (const char *)(*it));
        }
    }

    CommonSkeleton *ServiceManager::get_skeleton(const char *name) {
        auto it = this->skeletons.find(name);
        if (it == this->skeletons.end()) return NULL;
        return it->second;
    }

}

