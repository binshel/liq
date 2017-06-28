#ifndef DSF_COMMON_H_
#define DSF_COMMON_H_

#include <stdint.h>
#include <string>

#include "arduino_json/arduino_json.hpp"

using namespace ArduinoJson;

namespace liq {

#define MAX_NAME_LEN    256

    class CommonService {
        public:
            virtual int onload(libconfig::Setting &cfg) {
                return 0;
            }
            virtual int oninit(libconfig::Setting &cfg, std::map<std::string, CommonService*> &dep) {
                return 0;
            }
    };

    class CommonSkeleton {
        public:
            virtual void set_backend(CommonService *service) = 0;
            virtual google::protobuf::Message* handle(const char *method, uint8_t *reqBuff, int reqLen) = 0;
    };

    extern "C" {
        void liq_init(const char *cfgfile);
    }
}



#endif
