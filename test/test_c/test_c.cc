
#include <stdio.h>
#include <liq/logger.h>
#include <liq/timer.h>

#include "test/test_s.h"
#include "test/test_c.h"

namespace test {
    class SearchCImp : public SearchC{
    public:
        int onload(Json::Value &cfg) {
            // liq::Liq::instance().regist_tick_cb("search_c", std::bind(&SearchCImp::ontick, this));
            return 0;
        }
        int oninit(Json::Value &cfg, std::map<std::string, CommonService*> &deps) {
            this->server = (Search*)deps["server"];

            if (this->server) {
                liq::Liq::instance().timer->set_interval(5, [this]{
                            this->ontick();
                        });
            }
            return 0;
        }

        int ontick() {
            SearchRequest request;
            request.set_dd(23.23);
            SearchResponse *res = server->search(&request);
            if (res) {
                DEBUGF("ontick [%d][%s]", res->num(), res->reply().c_str());
            } else {
                DEBUGF("ontick [NULL]");
            }
            return 0;
        }

    private:
        Search *server;
    };

    extern "C" {
        CommonService* create_service() {
            return new SearchCImp();
        }
    }
}

