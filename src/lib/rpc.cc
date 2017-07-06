
#include "rpc.h"


namespace liq {

    RPC::RPC(ThreadPool *thread_pool, const char *from, const char *to)
        :thread_pool(thread_pool), from(from), to(to) {
    }

    uint8_t *RPC::alloc(int32_t len) {
        return buff;
    }

    void RPC::free(uint8_t *p) {
        return;
    }
    void RPC::call(const char *name, const uint8_t *req, int32_t reqLen,  uint8_t **resBuff, int32_t *resLen) {
    }

    uint8_t RPC::buff[1024];
}
