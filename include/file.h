#pragma once

#include "lib.h"

namespace liq {

    class IIO {
    public:
        virtual int32_t write(void *data, int32_t len) = 0;
        virtual int32_t read(void *buff, int32_t len) = 0;
        virtual int32_t close() = 0;
        virtual ~IIO() = 0;
    };

    class IOManager {
    public:
        IOManager() = delete;
        IOManager(LiqState* liq);

        IIO* open_file(const char *path);
        void close_file(IIO*);
        int32_t ontick();
        
    private:
        LiqState *liq;
        int epfd;
        const int EPOLL_SIZE = 1024;
        struct epoll_event events[EPOLL_SIZE];


    public:
        static inline int32_t new_index() {
            return ++index;

    private:
        static int32_t index;
    }

}
