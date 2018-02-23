#pragma once

#include <stdint.h>
#include <string>
#include <map>

#include "liq.h"
#include "logger.h"

namespace liq {

    class CommonService;

    typedef CommonService* (*fcn_create_service)();
    typedef void (*fcn_destory_service)(CommonService*);

    class Module {
    public: 
        Module(const std::string &path, void *handle);

        inline CommonService* create_service() {
            INFO << "create service" << file_path << EOL;
            if (!this->pcreate_service) {
                ERROR << "not symbol create_service" << file_path  << EOL;
                exit(-1);
            } else {
                return this->pcreate_service();
            }
        }
        inline void destory_service(CommonService* service) {
            if (!this->pdestory_service) {
                INFO << "not symbol destory_service" << file_path << EOL;
            } else {
                INFO << "destory service" << file_path << EOL;
                this->pdestory_service(service);
            }
        }

    protected:
        std::string file_path;
        fcn_create_service pcreate_service;
        fcn_destory_service pdestory_service;
        void *handle;
    };


    class ModuleManager {
    public:
        Module *load(const std::string &path);
        void load_dir(const std::string &path, const std::string &suf = ".so");

    private:
        Module *load_file(const std::string &path);
    private:
        std::map<std::string, Module*> modules;
    };
}
