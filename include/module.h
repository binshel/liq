#pragma once

#include <stdint.h>
#include <string>
#include <map>

#include "liq.h"

namespace liq {

    typedef CommonService* (*fcn_create_service)();
    typedef void (*fcn_destory_service)(CommonService*);

    struct Module {
        int32_t     id;
        std::string file_path;
        fcn_create_service create_service;
        fcn_destory_service destory_service;
        void *handle;
    };


    class ModuleManager {
    private:
        static std::map<std::string, Module*> modules;

    private:
        static Module *LoadFile(const std::string &path);
    public:
        static Module *Load(const std::string &path);
    };
}
