#ifndef DSF_COMMON_H_
#define DSF_COMMON_H_

#include <stdint.h>
#include <string>
#include <map>

#include <google/protobuf/message.h>
#include <arduino_json/arduino_json.hpp>

namespace liq {

#define MAX_NAME_LEN    256
    class ServiceManager;
    class ThreadPool;
    class ModuleManager;
    class RPC;
    class RPCManager;

    class ITickCB {
        friend class LiqState;
        public:
            ITickCB();
            virtual int ontick() = 0;
        protected:
            bool intick;
            static void call(ITickCB *cb);
    };
    
    class LiqState {
        public:
            LiqState(const char * node_name, const char *cfgfile);

            int ontick();
            void regist_tick_cb(const std::string &name, ITickCB *cb);

        public:
            ThreadPool *thread_pool;
            ServiceManager *service_manager;
            ModuleManager *module_manager;
            RPCManager *rpc_manager;
        private:
            std::string cfgfile;
            std::map<std::string, ITickCB*> tick_cbs;
    };

    class CommonService {
        public:
            virtual int onload(ArduinoJson::JsonObject &cfg);
            virtual int oninit(ArduinoJson::JsonObject &cfg, std::map<std::string, CommonService*> &deps);
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

    extern LiqState *liq;
    extern "C" {
        LiqState *liq_init(const char *node_name, const char *cfgfile);
    }
}


#endif
