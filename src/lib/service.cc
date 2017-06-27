
#include <string>

#include "service.h"


namespace liq {

    std::map<std::string, CommonService*> ServiceManager::services;
    std::map<std::string, CommonSkeleton*> ServiceManager::skeletons;


    void ServiceManager::LoadCfg(libconfig::Config &cfg) {
        libconfig::Setting &service_root = cfg.getRoot()["services"];
        libconfig::Setting &stub_root = cfg.getRoot()["services"];

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
            CommonService *stub = module->create_stub(stub_cfg["name"]);

            this->services[stub_cfg["name"]] = stub;
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
            CommonService *service = module->create_service(service_cfg["name"]);

            // create skeleton
            snprintf(name_buf, MAX_NAME_LEN, "%s.skeleton.so", service_cfg["module"]);
            Module *module = ModuleManager::Load(name_buf);
            CommonStub *stub = module->createSkeleton(service);

            this->services[service_cfg["name"]] = service;

            service->OnLoad(service_cfg);
        }

        // 设置依赖
    }

}

