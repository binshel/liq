#pragma once

#include <string>
#include <map>
#include <memory>

#include "module.h"
#include "rpc.h"

namespace liq {

    class ServiceManager
    {
    private:
        static std::map<std::string, CommonService*> services;
        static std::map<std::string, CommonSkeleton*> skeletons;

    public:
        static void load_cfg(LiqState* liq, ArduinoJson::JsonObject &cfg);
    };
}
