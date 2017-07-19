#pragma once

#include <sys/epoll.h>
#include <queue>

#include "liq.h"
#include "thread_pool.h"

namespace liq { namespace io {

    class IOManager;

    struct Event: public ThreadEvent {
        Event(int32_t type, void *buff, int32_t len, int32_t off = 0);
        ~Event();

        int64_t master;
        void *buff;
        int32_t len;
        int32_t off;
    };

    class IIO {
        friend class IOManager;
        protected:
        virtual int32_t on_in() = 0;
        virtual int32_t on_out() = 0;
        virtual int32_t on_err() = 0;

        int fd;
        int64_t master;
    };

    class File: public IIO {
        public:
            File();
            ~File();
            int32_t open(const char *path);
            void close();
            int32_t write(const void *data, int32_t len);
            int32_t write_sync(const void *data, int32_t len);
            int32_t read(void *data, int32_t len);

        protected:
            virtual int32_t on_in();
            virtual int32_t on_out();
            virtual int32_t on_err();

        private:
            int32_t write_buff(ThreadPool::EVENT type, const void *data, int32_t len);
            int32_t read_buff(ThreadPool::EVENT type, void *data, int32_t len);

        private:
            std::queue<Event*> write_queue;
            std::queue<Event*> read_queue;
    };

    class Tcp {
    };

    class Udp {
    };



    class IOManager {
    public:
        IOManager();

        int32_t ontick();
        int regist_io(IIO *io);
        int unregist_io(IIO *io);

    private:
        int epfd;
#define EPOLL_SIZE  1024
        struct epoll_event events[EPOLL_SIZE];

    public:
        static inline int64_t new_index() {
            return ++index;
        }
    private:
        static int64_t index;
    };
}}


