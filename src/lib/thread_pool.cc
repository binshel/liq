#include <string.h>
#include <ucontext.h>

#include <iostream>
#include <string>

#include "thread_pool.h"

int thread_base::now_id = 0;

thread_base::thread_base(thread_pool *belong)
{
    this->id = ++now_id;
    this->parent = NULL;
    bzero(this->stack, STACK_SIZE);

    getcontext(&this->ucp);
    this->ucp.uc_stack.ss_sp = this->stack;
    this->ucp.uc_stack.ss_size = STACK_SIZE;
    makecontext(&this->ucp, 
            (void(*)())thread_pool::thread_enter,
            (sizeof(belong) + sizeof(this)) / sizeof(int),
            belong, this);
}

