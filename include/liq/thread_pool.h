/**
 * @file thread_pool.h
 * @brief 协程创建与管理
 */


#pragma once

#include <ucontext.h>

#include <map>
#include <list>
#include <functional>

#include "liq.h"
#include "logger.h"


namespace liq {
    typedef std::function<void()> fun_spawn_cb;

    struct ThreadEvent {
        const std::string *err;
        void *data;
    };

    class ThreadPool;
    class Thread {
        friend class ThreadPool;
    private:
        Thread();
        void init();

        inline int64_t get_id()
        {
            return this->id;
        }
        inline void set_id(int64_t id)
        {
            this->id = id;
        }

        static void thread_enter(Thread *thread);

        int64_t id;
        Thread *parent;
        fun_spawn_cb enter;
        
        uint8_t stack[STACK_SIZE];
        ucontext_t ucp;
        int64_t yield_time;
        Thread *yield_pre;
        Thread *yield_next;
    };


    class ThreadPool {
        public:
            friend class Thread;
            ThreadPool();
            int32_t ontick();
            int64_t spawn(const fun_spawn_cb& enter);
            inline int64_t self() {
                return this->running->id;
            };

            ThreadEvent& yield(uint32_t timeout = 30);
            template <class T>
            int notify(int64_t tid, T *data) {
                auto it = this->threads.find(tid);
                if (it != this->threads.end()) {
                    auto thread = it->second;
                    this->notify_event.err = NULL;
                    this->notify_event.data = data;
                    this->swap_to(thread);
                    return 0;
                } else {
                    WARNF("notify thread not exist [%ld][0x%x]",
                            tid, data);
                    delete data;
                    return -1;
                }
            }

        private:
            inline int64_t alloc_id() {
                return ++now_id;
            }
            void free();
            void swap_to(Thread *to);
            Thread* get_free_thread();
            void ensure_pool();

            Thread **pool;
            int32_t pool_size;
            int32_t free_num;
            int32_t thread_num;
            std::map<int64_t, Thread*> threads;
            Thread *main_thread;
            Thread *running;
            ThreadEvent notify_event;
            int64_t tick_time;
            Thread *yields[THREAD_TIMEOUT_QUEUE_LENTH];

            static int64_t now_id;
    };
}
