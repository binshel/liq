#include <string>
#include <map>
#include <memory>
    
#include "liq/module.h"
#include "liq/rpc.h"
#include "liq/liq.h"
#include "liq/service.h"
#include "liq/logger.h"

namespace liq {


    ServiceManager::ServiceManager() 
    {
    }
    void ServiceManager::load_cfg(Json::Value &cfg)
    {
        // Liq::instance().module->load_dir("", ".pb.so");
        Liq::instance().module->load("liq.pb.so");
        Liq::instance().module->load("ezfun.pb.so");

        add_stubs(cfg["stubs"]);
        add_services(cfg["services"]);
        add_accepts(cfg["services"]);

        Liq::instance().thread_pool->spawn([this, &cfg](){
                this->loaded_cfg(cfg);
            });
    }
    void ServiceManager::loaded_cfg(Json::Value &cfg)
    {
        Json::Value &service_root = cfg["services"];

        // call onload
        for (auto it = service_root.begin(); it != service_root.end(); ++it) {
            const std::string &name = it.name();
            CommonService *service = services[name];
            INFOF("call onload %s", name.c_str());
            service->onload(*it);
        }
        // call oninit
        for (auto it = service_root.begin(); it != service_root.end(); ++it) {
            const std::string &name = it.name();
            auto service_cfg = *it;
            auto service = services[name];

            // 设置依赖
            Json::Value &dep_cfg = service_cfg["deps"];
            std::map<std::string, CommonService*> deps;
            for (auto it = dep_cfg.begin(); it != dep_cfg.end(); ++it) {
                deps[it.name()] = services[it->asString()];
            }
            INFOF("call oninit %s", name.c_str());
            service->oninit(service_cfg, deps);
        }
    }

    void ServiceManager::add_stubs(Json::Value &stub_root)
    {
        // 创建桩
        for (auto it = stub_root.begin(); it != stub_root.end(); ++it) {
            const std::string &name = it.name();
            auto stub_cfg = *it;
            char module_name[MAX_NAME_LEN];
            char name_buf[MAX_NAME_LEN];
            strncpy(module_name, stub_cfg["idl"].asCString(), MAX_NAME_LEN);
            for (int i = 0; module_name[i]; ++i) if (module_name[i] == '.') module_name[i] = '/';
            INFOF("stub name:[%s],module:[%s]", name.c_str(), module_name);

            // load pb
            snprintf(name_buf, MAX_NAME_LEN, "%s.pb.so", module_name);
            // Liq::instance().module->load(name_buf);
            const char *new_module_name = stub_cfg["idl"].asCString();
            INFOF("stub name:[%s],module:[%s]", name.c_str(), module_name, new_module_name);

            // create stub
            snprintf(name_buf, MAX_NAME_LEN, "%s.stub.so", module_name);
            Module *module = Liq::instance().module->load(name_buf);
            CommonService *service = module->create_service();
            CommonStub *stub = (CommonStub*)service;
            stub->set_rpc(Liq::instance().rpc->create_rpc(name.c_str(), stub_cfg["dst"].asCString()));
            services[name] = service;
        }
    }

    void ServiceManager::add_services(Json::Value &service_root) {
        // 创建服务和skeleton
        for (auto it = service_root.begin(); it != service_root.end(); ++it) {
            const std::string &name = it.name();
            auto service_cfg = *it;
            const char *module_name = service_cfg["service"].asCString();
            char name_buf[MAX_NAME_LEN];
            INFOF("service name:[%s],module:[%s]", name.c_str(), module_name);

            // load pb
            snprintf(name_buf, MAX_NAME_LEN, "%s.pb.so", module_name);
            // Liq::instance().module->load(name_buf);

            // create service
            snprintf(name_buf, MAX_NAME_LEN, "%s.so", module_name);
            Module *module = Liq::instance().module->load(name_buf);
            CommonService *service = module->create_service();
            services[name] = service;

            // create skeleton
            snprintf(name_buf, MAX_NAME_LEN, "%s.skeleton.so", module_name);
            module = Liq::instance().module->load(name_buf);
            CommonSkeleton *skeleton = (CommonSkeleton*)module->create_service();
            skeletons[name] = skeleton;
            skeleton->set_backend(service);
        }
    }

    void ServiceManager::add_accepts(Json::Value &service_root) {
        // 服务和 skeleton 已经创建，这里只需要创建到对端的管道
        for (auto it = service_root.begin(); it != service_root.end(); ++it) {
            auto service_cfg = *it;
            Json::Value &accept_root = service_cfg["cfg"]["accepts"];
            for (auto it = accept_root.begin(); it != accept_root.end(); ++it) {
                INFOF("[%s] accept:[%s]", it.name().c_str(), (*it)["name"].asCString());
                Liq::instance().rpc->create_ipc((*it)["name"].asCString());
            }
        }
    }

    CommonSkeleton *ServiceManager::get_skeleton(const char *name) {
        auto it = this->skeletons.find(name);
        if (it == this->skeletons.end()) return NULL;
        return it->second;
    }

}

