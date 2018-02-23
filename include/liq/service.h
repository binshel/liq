#pragma once

#include <jsoncpp/json/value.h>

#include "liq.h"

namespace liq {

    class CommonService {
        public:
            virtual int onload(Json::Value &cfg);
            virtual int oninit(Json::Value &cfg, std::map<std::string, CommonService*> &deps);
    };

    class CommonStub : public CommonService {
        protected:
            RPC *rpc;
        public:
            virtual void set_rpc(RPC *rpc);
    };

    class CommonSkeleton : public CommonService {
        public:
            virtual void set_backend(CommonService *service) = 0;
            virtual google::protobuf::Message* handle(const char *method, uint8_t *reqBuff, int reqLen) = 0;
    };

    class ServiceManager
    {
    public:
        ServiceManager();

        void load_cfg(Json::Value &cfg);
        CommonSkeleton* get_skeleton(const char *name);

    private:
        void loaded_cfg(Json::Value &cfg);
        void add_stubs(Json::Value &stub_root);
        void add_services(Json::Value &service_root);
        void add_accepts(Json::Value &accept_root);

        std::map<std::string, CommonService*> services;
        std::map<std::string, CommonSkeleton*> skeletons;

    };
}
