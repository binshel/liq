#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>

#include <liq/io.h>
#include <liq/logger.h>
#include <liq/timer.h>

#include "test/test_s.h"

namespace test {
    class SearchImp : public Search{
    public:
        int onload(Json::Value &cfg) {
            liq::Liq::instance().regist_tick_cb("search_s", std::bind(&SearchImp::ontick, this));

            len = 0;
            int port = 9892;
            int lfd;
            int ret = liq::IO::tcp_listen(port, &lfd);
            if (ret != LIQ_SUCCESS) {
                ERRORF("listen error %d %d:%d [%s]\n", port, ret, errno, strerror(errno));
                return -1;
            }
            liq::Liq::instance().io->regist_fd(lfd, 
                    std::bind(&SearchImp::onaccept, this, lfd));
            DEBUGF("listening in port %d:%d\n", port, lfd);
            return 0;
        }
        void onaccept(int lfd)
        {
            char ip[32];
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int pfd = accept(lfd, (struct sockaddr*)&peer, &len);
            DEBUGF("accepted %d:%d %s:%d\n", lfd, pfd, 
                    inet_ntop(AF_INET, &peer.sin_addr, ip, 32),
                    ntohs(peer.sin_port));

            liq::Liq::instance().io->regist_fd(pfd,
                    std::bind(&SearchImp::ondata, this, pfd));
        }

        void ondata(int fd)
        {
            ssize_t size = read(fd, buff + len, 1);
            if (size ==0) {
                DEBUGF("unregist_fd %d:%d:%s", fd, 
                        liq::Liq::instance().io->unregist_fd(fd),
                        strerror(errno));
                close(fd); 
            } else if(size < 0) {
                INFOF("read error %d:%d:%s", fd, 
                        liq::Liq::instance().io->unregist_fd(fd),
                        strerror(errno));
                close(fd); 
            } else {
                len += size;
                DEBUGF("test_s on read %d:%d len(%d):[%s]\n", fd, size, len, buff);
            }
            liq::Liq::instance().timer->set_interval(10, []() {
                        DEBUGF("timer interval ");
                    });
        }

        int ontick() {
            ++this->count;
            return 0;
        }

        virtual SearchResponse *search(string* name, A* a, 
                int32_t num, 
                google::protobuf::RepeatedField<int32_t>* nums, 
                google::protobuf::RepeatedPtrField<string>* strs, 
                int32_t i32, 
                int64_t i64, 
                uint32_t ui32, 
                uint64_t ui64, 
                double dd, 
                float ff, 
                bool bb, 
                ENUM ee, 
                string* str)
        {
            DEBUGF("get dd %lf", dd);
            SearchResponse *res = new SearchResponse();
            
            res->set_reply(buff, len);
            res->set_num(this->count);

            DEBUGF("readed(%d) [%s]", len, res->reply().c_str());
            
            return res;
        }

    private:
        int32_t count;
        char buff[1024];
        int len;

    };

    extern "C" {
        CommonService* create_service() {
            return new SearchImp();
        }
    }
}

