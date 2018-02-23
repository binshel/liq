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


    class IO {
        typedef std::function<void(int ev)> fun_fd_cb;
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

        int32_t ontick();
        int regist_fd(int fd, const fun_fd_cb &on_data);
        int unregist_fd(int fd);
        int update_fd(int fd, const fun_fd_cb &on_data);

        static int set_flags(int fd, int flags);
        static int tcp_listen(int port, int *lfd, const std::string *ip = NULL);

    private:
        int epfd;
        struct epoll_event events[EPOLL_SIZE];
        std::map<int, IOArgs*> registed;
    };


}


