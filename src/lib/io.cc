#include <fcntl.h>
#include <unistd.h>
#include <queue>

#include "liq/file.h"
#include "liq/liq.h"

/*

namespace liq {

    struct IOEvent: public ThreadEvent {
        int64_t master;
        void *buff;
        int32_t len;
        int32_t off;
        IOEvent(int32_t type, void *buff, int32_t len, int32_t off = 0)
                :type(type), buff(buff), len(len), off(off) {
            master = liq->thread_pool:self();
        }
        ~IOEvent() {
            switch(type) {
                case ThreadPool::EVENT_WRITE_SYNC :
                case ThreadPool::EVENT_READ_SYNC :
                    break;
                default:
                    free(buff);
            }
        }
    };

    class IOFile : public IIO {
    protected:
        IOFile(IOManager *manager)
            :liq(liq), manager(manager) {
        }

        inline int get_fd() {
            return fd;
        }
        
        int32_t open(const const char *path) {
            if fd >= 0 return -1;
            fd = ::open(path, O_RDWR | O_NONBLOCK);
            if (fd < 0) {
                return fd;
            } else {
                return 0;
            }
        }
        void close() {
            for (auto it = write_queue.begin(); it != write_queue.end(); ++it) {
                IOEvent *event = *it;
                if (event->type == ThreadPool::EVENT_WRITE_SYNC) {
                    event->value = fd;
                    liq->thread_pool->notify(event->master, event);
                } else {
                    delete event;
                }
            }
            for (auto it = write_queue.begin(); it != write_queue.end(); ++it) {
                IOEvent *event = *it;
                if (event->type == ThreadPool::EVENT_WRITE_SYNC) {
                    liq->thread_pool->notify(event->master, event);
                } else {
                    delete event;
                }
            }
            ::close(fd);
        }
        virtual ~IOFile() {
            this->close();
        }

        virtual int32_t write_buff(ThreadPool::EVENT type, const void *data, int32_t len) {
            int32_t off = 0;
            if (write_queue.empty()) {
                int32_t count = ::write(fd, data, len);
                if (count >= len || count < 0) return count;
                off = count;
            }
            auto buff = new IOEvent(liq, type, data, len, off);
            write_queue.push(buff);
            return off;
        }
        virtual int32_t write(const void *data, int32_t len) {
            return write_buff(ThreadPool::EVENT_WRITE, data, len);
        }
        virtual int32_t write_sync(const void *data, int32_t len) {
            auto count = write_buff(ThreadPool::EVENT_WRITE_SYNC, data, len);
            if (count >= len || count < 0) return count;
            IOEvent *buff = liq->thread_pool->yield(ThreadPool::EVENT_WRITE_SYNC, fd);
            int32_t count = buff->off;
            delete buff;
            return count
        }

        virtual int32_t read_buff(ThreadPool::EVENT type, const void *data, int32_t len) {
            int32_t off = 0;
            if (read_queue.empty()) {
                int32_t count = ::read(fd, data, len);
                if (count >= len || count < 0) return count;
                off = count;
            }
            auto buff = new IOEvent(liq, type, data, len, off);
            read_queue.push(buff);
            return off;
        }
        virtual int32_t read(void *data, int32_t len) {
            auto count = read_buff(ThreadPool::EVENT_READ_SYNC, data, len);
            if (count >= len || count < 0) return count;
            IOEvent *buff = liq->thread_pool->yield(ThreadPool::EVENT_READ_SYNC, fd);
            int32_t count = buff->off;
            delete buff;
            return count
        }

    private:
        int fd;
        int64_t master;
        IOManager *manager;
        std::queue<IOEvent*> write_queue;
        std::queue<IOEvent*> read_queue;
    }

    IOManager::IOManager() 
            : liq(liq) {
        epfd = epoll_create1(0);
    }

    IIO* IOManager::open_file(const char *path) {
        auto file = new IOFile(liq, this);
        auto fd = file->open(path);
        if (fd < 0) {
            free(file);
            return NULL;
        }
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLET;
        ev.data.ptr = file;
        epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
        return file;
    }
    void IOManager::close_file(IIO *io) {
        IOFile *file = io;
        epoll_ctl(epfd, EPOLL_CTL_DEL, file->get_fd(), NULL);
        delete file;
    }

    int32_t IOManager::ontick() {
        int count = epoll_wait(epfd, events, EPOLL_SIZE, 10);
        for (int i = 0; i < count; ++i) {
            if (
        }


        return count;
    }

}



*/










