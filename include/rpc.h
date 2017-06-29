#pragma once


namespace liq {

    class RPC {
        public:
            virtual uint8_t* getBuff(int32_t len) {
                return new uint8_t[len];
            }
            virtual uint8_t* releaseBuff(int8_t *buff) {
                delete buff;
            }
            virtual void call(const char *name, const uint8_t *req, int32_t reqLen,  uint8_t **resBuff, int32_t resLen) {
            }
    };
    
    class RPCManager {
        public:
            static RPC* createRPC(std::string &from, std::string &to) {
                return new RPC();
            }

    };
}
