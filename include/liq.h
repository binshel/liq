#ifndef DSF_COMMON_H_
#define DSF_COMMON_H_

#include <stdint.h>
#include <string>

#include <libconfig.h++>

namespace liq {

#define MAX_NAME_LEN    256

    class CommonSkeleton;

    class CommonService {
        protected:
            std::string name;
        public:
            virtual int OnLoad(libconfig::Setting &cfg) {
                return 0;
            }
            virtual int OnInit(libconfig::Setting &cfg, std::map<std::string, CommonService*> &dep) {
                return 0;
            }
    };

    class CommonSkeleton {
        protected:
            std::string name;
        public:
            virtual google::protobuf::Message* handle(const char *method, uint8_t *reqBuff, int reqLen) = 0;
    };

    class RPCBase {
        public:
            virtual uint8_t* getBuff(int32_t len) = 0;
            virtual uint8_t* releaseBuff(int32_t len) = 0;
            virtual void call(const char *name, const uint8_t *req, int32_t reqLen,  uint8_t **resBuff, int32_t resLen) = 0;
    };

    typedef CommonService* (*fcnCreateService)(std::string &name);
    typedef CommonStub* (*fcnCreateStub)(std::string &name);
    typedef CommonSkeleton* (*fcnCreateSkeleton)(CommonService *service);

    extern "C" {
        void liq_init(const char *cfgfile);
    }
}



#endif
