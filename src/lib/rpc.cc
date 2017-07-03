
#include <rpc.h>


namespace liq {

    uint8_t *RPC::alloc(int32_t len) {
        return RPC::Manager::buff;
    }

    void RPC::free(int8_t *p) {
        return;
    }
}
