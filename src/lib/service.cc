
#include <string>

#include "service.h"


namespace liq {

    std::map<std::string, CommonService*> ServiceManager::services;
    std::map<std::string, CommonSkeleton*> ServiceManager::skeletons;


    void ServiceManager::LoadCfg(JsonObject &cfg) {
        libconfig::Setting &service_root = cfg["services"];
        libconfig::Setting &stub_root = cfg["stubs"];

        // 创建桩
        for (int i = 0; i < stub_root.getLength(); ++i) {
            auto stub_cfg = stub_root[i];
            char name_buf[MAX_NAME_LEN];
            printf("stub name:[%s],module:[%s]\n", (const char*)stub_cfg["name"], (const char*)stub_cfg["module"]);

            // load pb
            snprintf(name_buf, MAX_NAME_LEN, "%s.pb.so", stub_cfg["module"]);
            Module *module = ModuleManager::Load(name_buf);

            // create stub
            snprintf(name_buf, MAX_NAME_LEN, "%s.stub.so", stub_cfg["module"]);
            Module *module = ModuleManager::Load(name_buf);
            CommonService *stub = module->create_module();
            this->services[stub_cfg["name"]] = stub;
            // stub->onload(stub_cfg);
        }

        // 创建服务和skeleton
        for (int i = 0; i < service_root.getLength(); ++i) {
            auto service_cfg = service_root[i];
            char name_buf[MAX_NAME_LEN];
            printf("service name:[%s],module:[%s]\n", (const char*)service_cfg["name"], (const char*)service_cfg["module"]);

            // load pb
            snprintf(name_buf, MAX_NAME_LEN, "%s.pb.so", service_cfg["module"]);
            Module *module = ModuleManager::Load(name_buf);

            // create service
            Module *module = ModuleManager::Load(service_cfg["module"]);
            CommonService *service = module->create_module();
            this->services[service_cfg["name"]] = service;
            service->onload(service_cfg);

            // create skeleton
            snprintf(name_buf, MAX_NAME_LEN, "%s.skeleton.so", service_cfg["module"]);
            Module *module = ModuleManager::Load(name_buf);
            CommonSkeleton *skeleton = module->create_module();
            this->skeletons[name_buf] = skeleton;
            skeleton->set_backend(service);
        }

        // 设置依赖
        for (int i = 0; i < service_root.getLength(); ++i) {
            auto service_cfg = service_root[i];
            auto dep_cfg = service_cfg["dep"];
        }
    }

}

