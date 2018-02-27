/**
 * @file io.h
 * @brief 提供 IO（文件、socket）功能
 */

#pragma once

#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>

#include <queue>

#include "config.h"
#include "liq.h"
#include "thread_pool.h"



namespace liq {


    /**
     * 管理文件描述符，提供 IO 功能
     */
    class IO {
        /// @brief 时间发生后的回调函数类型
        typedef std::function<void(int ev)> fun_fd_cb;

        /**
         * @brief 文件描述符对应的参数 
         */
        class IOArgs {
        public:
            inline IOArgs(const fun_fd_cb &on_data)
                :on_data(on_data)
            {
            }

            fun_fd_cb on_data;
        };
    public:
        IO();

        /// @brief 被调用来处理一次 IO 工作
        int32_t ontick();

        /** 
         * @brief 注册被管理的文件描述符
         * @param fd 文件描述符
         * @param on_data 文件有数据可读时被回调的函数
         */
        int regist_fd(int fd, const fun_fd_cb &on_data);

        /**
         * @brief 取消被注册了的文件描述符
         * @param fd 要注册的文件描述符
         */
        int unregist_fd(int fd);

        /**
         * @brief 更新文件描述符的回调函数
         * @param fd 被更行的文件描述符
         * @param on_data 新的回调函数
         */
        int update_fd(int fd, const fun_fd_cb &on_data);

        /**
         * @brief 这只文件描述符的flag（标签）
         * @param fd 被操做的文件描述符
         * @param flags 设置的flag
         */
        static int set_flags(int fd, int flags);

        /**
         * @brief 创建一个监听socket
         * @param port 监听的端口
         * @param lfd 成功创建是接受创建的描述符
         * @param ip（可选） 监听的 IP
         * @details 创建的 socket 没有被注册，如需要调用注册接口
         */
        static int tcp_listen(int port, int *lfd, const std::string *ip = NULL);

    private:
        int epfd;   ///< epool 文件描述符
        struct epoll_event events[EPOLL_SIZE];      ///< 有时间发生的描述符数组
        std::map<int, IOArgs*> registed;            ///< 已注册的文件描述符
    };


}


