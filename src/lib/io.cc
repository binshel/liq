#include <fcntl.h>
#include <unistd.h>

#include "liq/io.h"
#include "liq/liq.h"


namespace liq { namespace io {

    Event::Event(int32_t type, void *buff, int32_t len, int32_t off)
        :buff(buff), len(len), off(off)
    {
        type = type;
        master = liq->thread_pool->self();
    }
    Event::~Event() {
        switch(type) {
        case ThreadPool::EVENT_WRITE_SYNC:
        case ThreadPool::EVENT_READ_SYNC:
            break;
        default:
            free(buff);
        }
    }

    File::File() {
        fd = -1;
    }
    File::~File() {
        this->close();
    }

    int32_t File::open(const char *path) {
        if (fd >= 0) return -1;
        fd = ::open(path, O_RDWR | O_NONBLOCK);
        if (fd < 0) {
            liq->io_manager->regist_io(this);
            return fd;
        } else {
            return -1;
        }
    }
    void File::close() {
        liq->io_manager->unregist_io(this);
        for (; !write_queue.empty(); write_queue.pop()) {
            Event *event = write_queue.front();
            if (event->type == ThreadPool::EVENT_WRITE_SYNC) {
                event->value = fd;
                event->off = -1;
                liq->thread_pool->notify(event->master, event);
            } else {
                delete event;
            }
        }
        for (; !read_queue.empty(); read_queue.pop()) {
            Event *event = read_queue.front();
            if (event->type == ThreadPool::EVENT_READ_SYNC) {
                event->value = fd;
                event->off = -1;
                liq->thread_pool->notify(event->master, event);
            } else {
                delete event;
            }
        }
        ::close(fd);
    }

    int32_t File::write_buff(ThreadPool::EVENT type, const void *data, int32_t len) {
        int32_t off = 0;
        if (write_queue.empty()) {
            int32_t count = ::write(fd, data, len);
            if (count >= len || count < 0) return count;
            off = count;
        }
        auto buff = new Event(type, (void*)data, len, off);
        write_queue.push(buff);
        return off;
    }

    int32_t File::write(const void *data, int32_t len) {
        return write_buff(ThreadPool::EVENT_WRITE, data, len);
    }

    int32_t File::write_sync(const void *data, int32_t len) {
        auto count = write_buff(ThreadPool::EVENT_WRITE_SYNC, data, len);
        if (count >= len || count < 0) return count;
        Event *buff = (Event*)liq->thread_pool->yield(ThreadPool::EVENT_WRITE_SYNC, fd);
        count = buff->off;
        delete buff;
        return count;
    }

    int32_t File::read_buff(ThreadPool::EVENT type, void *data, int32_t len) {
        int32_t off = 0;
        if (read_queue.empty()) {
            int32_t count = ::read(fd, data, len);
            if (count >= len || count < 0) return count;
            off = count;
        }
        auto buff = new Event(type, data, len, off);
        read_queue.push(buff);
        return off;
    }

    int32_t File::read(void *data, int32_t len) {
        auto count = read_buff(ThreadPool::EVENT_READ_SYNC, data, len);
        if (count >= len || count < 0) return count;
        Event *buff = (Event*)liq->thread_pool->yield(ThreadPool::EVENT_READ_SYNC, fd);
        count = buff->off;
        delete buff;
        return count;
    }

    int32_t File::on_in() {
    }
    int32_t File::on_out() {
    }
    int32_t File::on_err() {
    }


    IOManager::IOManager()  {
        epfd = epoll_create1(0);
    }

    int IOManager::regist_io(IIO *io) {
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLET;
        ev.data.ptr = io;
        return epoll_ctl(epfd, EPOLL_CTL_ADD, io->fd, &ev);
    }

    int IOManager::unregist_io(IIO *io) {
        return epoll_ctl(epfd, EPOLL_CTL_DEL, io->fd, NULL);
    }

    int32_t IOManager::ontick() {
        int count = epoll_wait(epfd, events, EPOLL_SIZE, 10);
        for (int i = 0; i < count; ++i) {
            IIO *io = (typeof(io))(events[i].data.ptr);
            if (events[i].events & EPOLLIN) io->on_in();
            if (events[i].events & EPOLLOUT) io->on_out();
            if (events[i].events & EPOLLERR) io->on_err();
        }
        return count;
    }

}}












