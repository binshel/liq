#ifndef DSF_COMMON_H_
#define DSF_COMMON_H_

#include <stdint.h>
#include <string>
#include <map>

#include <arduino_json/arduino_json.hpp>

namespace liq {

#define MAX_NAME_LEN    256
    class ServiceManager;
    class ThreadPool;

    class ITickCB {
        public:
            virtual int ontick() = 0;
    };
    
    class LiqState {
        public:
            LiqState(const char *cfgfile);

            int ontick();
            void regist_tick_cb(const std::string &name, ITickCB *cb);

        public:
            ThreadPool *thread_pool;
            ServiceManager *service_manager;
        private:
            std::string cfgfile;
            std::map<std::string, ITickCB*> tick_cbs;
    };

    class CommonService {
        public:
            virtual int onload(LiqState *liq, ArduinoJson::JsonObject &cfg);
            virtual int oninit(ArduinoJson::JsonObject &cfg, std::map<std::string, CommonService*> &deps);
    };

    extern "C" {
        LiqState *liq_init(const char *cfgfile);
    }
}


#endif
