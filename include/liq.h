#ifndef DSF_COMMON_H_
#define DSF_COMMON_H_

#include <stdint.h>
#include <string>
#include <map>

#include <google/protobuf/message.h>
#include <arduino_json/arduino_json.hpp>

namespace liq {

#define MAX_NAME_LEN    256

    class CommonService {
        public:
            virtual int onload(ArduinoJson::JsonObject &cfg) {
                return 0;
            }
            virtual int oninit(ArduinoJson::JsonObject &cfg, std::map<std::string, CommonService*> &deps) {
                return 0;
            }
    };

    class CommonSkeleton : public CommonService {
        public:
            virtual void set_backend(CommonService *service) = 0;
            virtual google::protobuf::Message* handle(const char *method, uint8_t *reqBuff, int reqLen) = 0;
    };

}

extern "C" {
    void liq_init(const char *cfgfile);
}

#endif
