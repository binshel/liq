#include <string.h>
#include <ucontext.h>

#include <iostream>
#include <string>

#include "liq/thread_pool.h"

namespace liq {

    int64_t ThreadBase::now_id = 0;

    ThreadBase::ThreadBase(ThreadPool *belong) {
        this->id = ++now_id;
        this->parent = NULL;
        this->is_yield = false;
        bzero(this->stack, STACK_SIZE);

        getcontext(&this->ucp);
        this->ucp.uc_stack.ss_sp = this->stack;
        this->ucp.uc_stack.ss_size = STACK_SIZE;
        makecontext(&this->ucp, 
                (void(*)())ThreadPool::thread_enter,
                (sizeof(belong) + sizeof(this)) / sizeof(int),
                belong, this);
    }
    
    void ThreadBase::clear() {
        id = 0; 
        enter = NULL;
        parent = NULL; 
        for (auto it = events.begin(); it != events.end(); ++it)
            delete *it;
        events.clear();
    }

    bool ThreadBase::can_active(ThreadEvent *event) {
        return this->is_yield 
                && (this->yield_type == 0 || this->yield_type == event->type)
                && (this->yield_value == 0 || this->yield_value == event->value);
    }


    ThreadPool::ThreadPool() :
        pool(NULL),
        pool_size(0),
        free_num(0),
        thread_num(0),
        threads()
    {
        this->main_thread = new ThreadBase(this);
        this->running = this->main_thread;
    }
    int64_t ThreadPool::spawn(ThreadBase::fun_enter enter, void* args)
    {
        ThreadBase *thread = this->get_free_thread();
        int64_t id = thread->id;
        thread->enter = enter;
        thread->args = args;
        thread->parent = this->running;
        this->threads[id] = thread;
        this->swap_to(thread);
        return id;
    }

    ThreadEvent* ThreadPool::yield(int32_t type, int32_t value)
    {
        auto thread = this->running;
        thread->is_yield = true;
        thread->yield_type = type;
        thread->yield_value = value;

        for (auto it = thread->events.begin(); it != thread->events.end(); ++it) {
            if (thread->can_active(*it)) {
                auto event = *it;
                thread->events.erase(it);
                thread->is_yield = false;
                return event;
            } 
        }
        this->swap_to(this->running->parent);
        thread->is_yield = false;
        return this->notify_event;
    }
    int ThreadPool::notify(int64_t id, ThreadEvent *event)
    {
        auto it = this->threads.find(id);
        if (it != this->threads.end()) {
            auto thread = it->second;
            if (thread->can_active(event)) {
                this->notify_event = event;
                this->swap_to(it->second);
            } else {
                thread->events.push_back(event);
            }
            return 0;
        } else {
            return -1;
        }
    }

    void ThreadPool::free()
    {
        this->ensure_pool();
        this->pool[this->free_num] = this->running;
        ++this->free_num;
        
        this->threads.erase(this->running->id);
        ThreadBase *parent = this->running->parent;
        this->running->clear();
        this->swap_to(parent);
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
