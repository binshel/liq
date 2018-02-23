#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "config.h"
#include "liq/liq.h"
#include "liq/logger.h"
#include "liq/io.h"


namespace liq { 

    /*
    void * malloc(int size)
    {
        return yesmalloc(size);
    }
    void free(void * ptr)
    {
        return yesfree(ptr);
    }
    */

 
    IO::IO() : registed()
    {
        epfd = epoll_create1(0);
    }

    int IO::regist_fd(int fd, const fun_fd_cb &on_data)
    {
        if (registed.find(fd) != registed.end()) {
            return LIQ_ERR_IO_REGISTED;
        }
        int ret = IO::set_flags(fd, O_NONBLOCK);
        if (ret != LIQ_SUCCESS) {
            return ret;
        }

        struct epoll_event ev;
        IOArgs *args = new IOArgs(on_data);
        ev.events = EPOLLIN | EPOLLHUP | EPOLLRDHUP| EPOLLERR | EPOLLET;
        ev.data.ptr = args;
        ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
        if (ret != 0) {
            ERRORF("epoll error %d:%d [%s]\n", epfd, fd, strerror(errno));
            delete args;
            return LIQ_ERR_IO_CANNT_REGIST;
        } 
        registed[fd] = args;
        return LIQ_SUCCESS;
    }

    int IO::unregist_fd(int fd)
    {
        auto it = registed.find(fd);
        if (it == registed.end()) {
            return LIQ_ERR_IO_UNREGISTED;
        }
        delete it->second;
        registed.erase(it);
        return epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
    }
    int IO::update_fd(int fd, const fun_fd_cb &on_data)
    {
        auto it = registed.find(fd);
        if (it == registed.end()) {
            return LIQ_ERR_IO_UNREGISTED;
        }
        it->second->on_data = on_data;
        return LIQ_SUCCESS;
    }

    int32_t IO::ontick() {
        int count = epoll_wait(epfd, events, EPOLL_SIZE, 1);
        if (count < 0) return 0;
        for (int i = 0; i < count; ++i) {
            struct epoll_event &ev = events[i];
            IOArgs *args = static_cast<IOArgs*>(ev.data.ptr);
            DEBUGF("epoll events 0x%x", ev.events);
            Liq::instance().thread_pool->spawn([&ev, args]() {
                        args->on_data(ev.events);
                    });
        }
        return count;
    }
    int IO::set_flags(int fd, int flags)
    {
        int old = fcntl(fd, F_GETFL, 0);
        if (old < 0) {
            return LIQ_ERR_FCNTL_ERROR;
        }
        int cur = old | flags;
        cur = fcntl(fd, F_SETFL, cur);
        if (cur < 0) {
            return LIQ_ERR_FCNTL_ERROR;
        }
        return LIQ_SUCCESS;
    }
    int IO::tcp_listen(int port, int *lfd, const std::string *ip)
    {
        int optval = 1;
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(port);

        int fd = socket(AF_INET, SOCK_STREAM, PF_UNSPEC);
        if (fd < 0) {
            return LIQ_ERR_SOCK_NEW;
        }

        int ret = LIQ_SUCCESS;
        ret = setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
        if (ret != 0) {
            ret = LIQ_ERR_SOCK_OPT;
            goto out;
        }
        ret = bind(fd, (struct sockaddr *)(&addr), sizeof(addr));
        if (ret != 0) {
            ret = LIQ_ERR_SOCK_BIND;
            goto out;
        }
        ret = listen(fd, LISTEN_QUEUE);
        if (ret != 0) {
            ret = LIQ_ERR_SOCK_LISTEN;
            goto out;
        }
        *lfd = fd;
        return LIQ_SUCCESS;
out:
        close(fd);
        return ret;
        
    }
}












