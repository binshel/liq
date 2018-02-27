/**
 * @file config.h
 * @brief 定义印象程序构建的参数宏
 */

#pragma once


#define MAX_NAME_LEN    256     ///< 服务名、模块名等的最大长度
#define EPOLL_SIZE  1024        ///< epoll_wait 返回的最大数组长度

#define STACK_SIZE  (32 * 1024)     ///< 协程的栈大小
#define THREAD_TIMEOUT_QUEUE_LENTH   (60)       ///< 协程等待超时的 hash 队列长度

#define LISTEN_QUEUE        10      ///< 监听套接字等待连接队列的最大长度
