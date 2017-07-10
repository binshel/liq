#pragma once

#include <ucontext.h>

#include <map>


namespace liq {

#define STACK_SIZE  (32 * 1024)

    class ThreadPool;

    class ThreadBase {
        public:
            typedef void (*fun_enter)(void*);
            friend class ThreadPool;
            ThreadBase() = delete;
            ThreadBase(ThreadPool *belong);

        private:
            static int32_t now_id;
            int32_t id;
            fun_enter enter;
            void *args;

            ucontext_t ucp;
            ThreadBase *parent;
            uint8_t stack[STACK_SIZE];

    };


    class ThreadPool {
        public:
            enum EVENT {
                EVENT_RPC       = 1
            };

            friend class ThreadBase;
            ThreadPool();

            void spawn(ThreadBase::fun_enter enter, void* args);

            void* yield(int32_t event, int32_t value);
            int notify(int32_t event, int32_t value, void *args);

        private:
            void free();
            void swap_to(ThreadBase *to);
            ThreadBase* get_free_thread();
            void ensure_pool();
            static void thread_enter(ThreadPool *pool, ThreadBase *thread);

            ThreadBase **pool;
            int32_t pool_size;
            int32_t thread_num;
            int32_t free_num;
            ThreadBase *running;
            ThreadBase *main_thread;
            std::map<int64_t, ThreadBase*> suspends;
            void *notify_args;
    };
}
