#include <string.h>
#include <ucontext.h>

#include <iostream>
#include <string>

#include "thread_pool.h"

namespace liq {

    int ThreadBase::now_id = 0;

    ThreadBase::ThreadBase(ThreadPool *belong) {
        this->id = ++now_id;
        this->parent = NULL;
        bzero(this->stack, STACK_SIZE);

        getcontext(&this->ucp);
        this->ucp.uc_stack.ss_sp = this->stack;
        this->ucp.uc_stack.ss_size = STACK_SIZE;
        makecontext(&this->ucp, 
                (void(*)())ThreadPool::thread_enter,
                (sizeof(belong) + sizeof(this)) / sizeof(int),
                belong, this);
    }


    ThreadPool::ThreadPool() :
        pool(NULL),
        pool_size(0),
        free_num(0),
        thread_num(0),
        suspends()
    {
        this->main_thread = new ThreadBase(this);
        this->running = this->main_thread;
    }
    int32_t ThreadPool::spawn(void(*enter)(void*), void* args)
    {
        ThreadBase *thread = this->get_free_thread();
        thread->enter = enter;
        thread->args = args;
        this->resume(thread);
    }

    void* ThreadPool::yield(int32_t event, int32_t value)
    {
        int64_t sig = event << 32 + value;
        this->suspends[sig] = this->running;
        this->swap_to(this->running->parent);
        return this->notify_args;
    }
    int ThreadPool::notify(int32_t event, int32_t value, void *args)
    {
        int64_t sig = event << 32 + value;
        int32_t count = 0;
        auto it_thread = this->suspends.find(sig);
        if (it_thread != this->suspends.end()) {
            ++count;
            this->notify_args = args;
            this->suspends.erase(it_thread);
            this->swap_to(it_thread->second);
        }
        return count;
    }

    void ThreadPool::resume(ThreadBase *thread)
    {
        thread->parent = this->running;
        this->swap_to(thread);
    }

    void ThreadPool::free()
    {
        this->ensure_pool();
        this->pool[this->free_num] = this->running;
        ++this->free_num;
        this->swap_to(this->running->parent);
    }

    void ThreadPool::swap_to(ThreadBase *to)
    {
        ThreadBase *from = this->running;
        this->running = to;
        swapcontext(&from->ucp, &to->ucp);
    }

    ThreadBase* ThreadPool::get_free_thread()
    {
        if (this->free_num > 0) {
            --this->free_num;
            return this->pool[this->free_num];
        } else {
            ThreadBase *thread = new ThreadBase(this);
            ++this->thread_num;
            return thread;
        }
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

    void ThreadPool::thread_enter(ThreadPool *pool, ThreadBase *thread)
    {
        while (true) {
            thread->enter(thread->args);
            pool->free();
        }
    }

}
