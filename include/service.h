#pragma once

#include <string>
#include <map>
#include <memory>

#include "module.h"

namespace liq {

    class ServiceManager
    {
    private:
        static std::map<std::string, CommonService*> services;
        static std::map<std::string, CommonSkeleton*> skeletons;

    public:
        static void LoadCfg(JsonObject &cfg);
    };

    typedef void* (*fcn_create_module)(std::string &name);
    typedef void* (*fcnCreateStub)(std::string &name);
    typedef CommonSkeleton* (*fcnCreateSkeleton)(CommonService *service);
}
