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
    public:
        ServiceManager() = delete;
        ServiceManager(ArduinoJson::JsonObject &cfg);

        CommonSkeleton* get_skeleton(const char *name);

    private:
        void add_stubs(ArduinoJson::JsonObject &stub_root);
        void add_services(ArduinoJson::JsonObject &service_root);
        void add_accepts(ArduinoJson::JsonArray &accept_root);

        std::map<std::string, CommonService*> services;
        std::map<std::string, CommonSkeleton*> skeletons;

    };
}
