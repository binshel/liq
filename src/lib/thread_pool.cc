#include <stdlib.h>
#include <string.h>
#include <ucontext.h>

#include <iostream>
#include <string>

#include "liq/thread_pool.h"
#include "liq/liq.h"
#include "liq/logger.h"
#include "liq/error.h"

namespace liq {


    Thread::Thread() 
    {
        bzero(this->stack, STACK_SIZE);

        getcontext(&this->ucp);
        this->ucp.uc_stack.ss_sp = this->stack;
        this->ucp.uc_stack.ss_size = STACK_SIZE;
        makecontext(&this->ucp, 
                (void(*)())Thread::thread_enter,
                sizeof(this) / sizeof(int),
                this);
    }
    void Thread::init()
    {
        this->id = Liq::instance().thread_pool->alloc_id();
        this->parent = NULL;
        // this->enter = NULL;
    }

    void Thread::thread_enter(Thread *thread)
    {
        while (true) {
            try {
                (thread->enter)();
            } catch (const char *err) {
                ERRORF("thread catch error %s", err);
            } catch (...) {
                ERRORF("thread unknow error");
            }
            Liq::instance().thread_pool->free();
        }
    }
    
    int64_t ThreadPool::now_id = 0;
    ThreadPool::ThreadPool() :
        pool(NULL),
        pool_size(0),
        free_num(0),
        thread_num(0),
        threads()
    {
        this->tick_time = Liq::instance().now.tv_sec;
        memset(this->yields, 0, sizeof(this->yields));
        this->main_thread = this->get_free_thread();
        this->running = this->main_thread;
    }
    int32_t ThreadPool::ontick() {
        int count = 0;
        for(int64_t time = this->tick_time + 1; time <= Liq::instance().now.tv_sec; ++time) {
            int index = time % THREAD_TIMEOUT_QUEUE_LENTH;
            auto thread = this->yields[index];
            while (thread) {
                auto tmp = thread->yield_next;
                if (thread->yield_time <= time) {
                    ++count;
                    this->notify_event.err = &LIQ_YIELD_TIMEOUT;
                    this->swap_to(thread);
                }
                thread = tmp;
            }
        }
        this->tick_time = Liq::instance().now.tv_sec;
        return count;
    }
    int64_t ThreadPool::spawn(const fun_spawn_cb& enter)
    {
        Thread *thread = this->get_free_thread();
        int64_t id = thread->id;
        thread->enter = enter;
        thread->parent = this->running;
        this->threads[id] = thread;
        this->swap_to(thread);
        return id;
    }

    ThreadEvent& ThreadPool::yield(uint32_t timeout)
    {
        if (this->running == this->main_thread) {
            ERRORF("main thread be yield");
            abort();
        }

        auto thread = this->running;

        int index;
        if (timeout > 0) {
            thread->yield_time = Liq::instance().now.tv_sec + timeout;
            index = thread->yield_time % THREAD_TIMEOUT_QUEUE_LENTH;
            auto p = this->yields[index];
            this->yields[index] = thread;
            thread->yield_pre = NULL;
            thread->yield_next = p;
            if (p) {
                p->yield_pre = thread;
            }
        }
        this->swap_to(this->running->parent);
        if (timeout > 0) {
            if(thread->yield_pre) {
                thread->yield_pre->yield_next = thread->yield_next;
            } else {
                this->yields[index] = thread->yield_next;
            }
            if (thread->yield_next) {
                thread->yield_next->yield_pre = thread->yield_pre;
            }
        }
        thread->yield_pre = NULL;
        thread->yield_next = NULL;
        return this->notify_event;
    }
    /*
    int ThreadPool::notify(int64_t tid, void *data)
    {
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
            // TODO 使用
            delete data;
            return -1;
        }
    }
    */

    void ThreadPool::free()
    {
        this->ensure_pool();
        this->pool[this->free_num] = this->running;
        ++this->free_num;
        
        this->threads.erase(this->running->id);
        Thread *parent = this->running->parent;
        this->swap_to(parent);
    }

    void ThreadPool::swap_to(Thread *to)
    {
        Thread *from = this->running;
        this->running = to;
        swapcontext(&from->ucp, &to->ucp);
    }

    Thread* ThreadPool::get_free_thread()
    {
        Thread *thread;
        if (this->free_num > 0) {
            --this->free_num;
            thread = this->pool[this->free_num];
        } else {
            thread = new Thread();
            ++this->thread_num;
        }
        thread->init();
        return thread;
    }

    void ThreadPool::ensure_pool()
    {
        if (this->free_num < this->pool_size) {
            return;
        }
        if (this->pool) {
            this->pool_size *= 2;
            this->pool = (decltype(this->pool))realloc(this->pool, 
                    sizeof(*this->pool) * this->pool_size);
        } else {
            this->pool_size = 2;
            this->pool = (decltype(this->pool))malloc(sizeof(*this->pool) * this->pool_size);
        }
    }


}
