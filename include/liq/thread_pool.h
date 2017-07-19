#pragma once

#include <ucontext.h>

#include <map>
#include <list>


namespace liq {

#define STACK_SIZE  (32 * 1024)

    class ThreadPool;

    struct ThreadEvent {
        int32_t type;
        int32_t value;
    };

    class ThreadBase {
        public:
            typedef void (*fun_enter)(void*);
            friend class ThreadPool;
            ThreadBase() = delete;
            ThreadBase(ThreadPool *belong);

            void clear();
            bool can_active(ThreadEvent *event);

        private:
            int64_t id;
            fun_enter enter;
            void *args;
            ucontext_t ucp;
            ThreadBase *parent;
            uint8_t stack[STACK_SIZE];
            std::list<ThreadEvent*> events;
            bool    is_yield;
            int32_t yield_type;
            int32_t yield_value;

        private:
            static int64_t now_id;
    };


    class ThreadPool {
        public:
            enum EVENT {
                EVENT_RPC           = 1,
                EVENT_WRITE         = 2,
                EVENT_WRITE_SYNC    = 3,
                EVENT_READ_SYNC     = 4
            };

            friend class ThreadBase;
            ThreadPool();

            int64_t spawn(ThreadBase::fun_enter enter, void* args);
            inline int64_t self() {
                return this->running->id;
            };

            ThreadEvent* yield(int32_t type = 0, int32_t value = 0);
            int notify(int64_t id, ThreadEvent *event);

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
            std::map<int64_t, ThreadBase*> threads;
            ThreadEvent *notify_event;
    };
}
