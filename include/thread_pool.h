#pragma once

#include <ucontext.h>

#include <map>


#define STACK_SIZE  (8 * 1024)

class thread_pool;

class thread_base
{
    friend class thread_pool;
public:
    thread_base() = delete;
    thread_base(thread_pool *belong);

private:
    static int32_t now_id;
    int32_t id;
    void (*enter)(void*);
    void *args;

    ucontext_t ucp;
    thread_base *parent;
    uint8_t stack[STACK_SIZE];

};



class thread_pool
{
public:
    friend class thread_base;
    thread_pool() :
        pool(NULL),
        pool_size(0),
        free_num(0),
        thread_num(0),
        suspends()
    {
        this->main_thread = new thread_base(this);
        this->running = this->main_thread;
    }

    int32_t spawn(void(*enter)(void*), void* args)
    {
        thread_base *thread = this->get_free_thread();
        thread->enter = enter;
        thread->args = args;
        this->resume(thread);
    }

    void* yield(int32_t event, int32_t value)
    {
        int64_t sig = event << 32 + value;
        this->suspends[sig] = this->running;
        this->swap_to(this->running->parent);
        return this->notify_args;
    }
    int notify(int32_t event, int32_t value, void *args)
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

private:
    void resume(thread_base *thread)
    {
        thread->parent = this->running;
        this->swap_to(thread);
    }

    void free()
    {
        this->ensure_pool();
        this->pool[this->free_num] = this->running;
        ++this->free_num;
        this->swap_to(this->running->parent);
    }

    void swap_to(thread_base *to)
    {
        thread_base *from = this->running;
        this->running = to;
        swapcontext(&from->ucp, &to->ucp);
    }

    thread_base* get_free_thread()
    {
        if (this->free_num > 0) {
            --this->free_num;
            return this->pool[this->free_num];
        } else {
            thread_base *thread = new thread_base(this);
            ++this->thread_num;
            return thread;
        }
    }

    void ensure_pool()
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

    static void thread_enter(thread_pool *pool, thread_base *thread)
    {
        while (true) {
            thread->enter(thread->args);
            pool->free();
        }
    }

private:
    thread_base **pool;
    int32_t pool_size;
    int32_t thread_num;
    int32_t free_num;
    thread_base *running;
    thread_base *main_thread;
    std::map<int64_t, thread_base*> suspends;
    void *notify_args;


};
