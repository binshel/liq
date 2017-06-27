#pragma once

#include <string>
#include <map>
#include <memory>

#include <libconfig.h++>

#include "module.h"

namespace liq {

    class ServiceManager
    {
    private:
        static std::map<std::string, CommonService*> services;
        static std::map<std::string, CommonSkeleton*> skeletons;

    public:
        static void LoadCfg(libconfig::Config &cfg);
    };

    typedef CommonService* (*fcnCreateService)(std::string &name);
    typedef CommonStub* (*fcnCreateStub)(std::string &name);
    typedef CommonSkeleton* (*fcnCreateSkeleton)(CommonService *service);
}
