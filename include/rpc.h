#pragma once

#include <google/protobuf/message.h>

#include "liq.h"

namespace liq {

    class RPC {
        public:
            virtual uint8_t* alloc(int32_t len);
            virtual void free(uint8_t *buff);
            virtual void call(const char *name, const uint8_t *req, int32_t reqLen,  uint8_t **resBuff, int32_t *resLen);
    };
    
    class RPCManager {
        public:
            static uint8_t *get_buff();
            static RPC* create_rpc(const char *from, const char *to) {
                return new RPC();
            }
        private:
            static uint8_t buff[1024];

    };

    class CommonStub : public CommonService {
        protected:
            RPC *rpc;
        public:
            virtual void set_rpc(RPC *rpc) {
                this->rpc = rpc;
            }
    };

    class CommonSkeleton : public CommonService {
        public:
            virtual void set_backend(CommonService *service) = 0;
            virtual google::protobuf::Message* handle(const char *method, uint8_t *reqBuff, int reqLen) = 0;
    };

}
