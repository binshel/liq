#pragma once

#include <string>
#include <map>
#include <memory>

#include "module.h"
#include "rpc.h"
#include "liq.h"

namespace liq {

    class ServiceManager
    {
    private:
        std::map<std::string, CommonService*> services;
        std::map<std::string, CommonSkeleton*> skeletons;

    public:
        ServiceManager() = delete;
        ServiceManager(LiqState* liq, ArduinoJson::JsonObject &cfg);
    };
}
