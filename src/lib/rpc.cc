
#include <rpc.h>


namespace liq {

    uint8_t *RPC::alloc(int32_t len) {
        return RPCManager::get_buff();
    }

    void RPC::free(uint8_t *p) {
        return;
    }
    void RPC::call(const char *name, const uint8_t *req, int32_t reqLen,  uint8_t **resBuff, int32_t *resLen) {
    }


    uint8_t RPCManager::buff[1024];
    uint8_t *RPCManager::get_buff() {
        return buff;
    }
}
