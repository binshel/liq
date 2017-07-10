
#include <stdio.h>
#include "pb/test_s.h"
#include "pb/test_c.h"

namespace test {
    class SearchC : public liq::ITickCB, public SearchCService {
    public:
        int onload(LiqState *liq, ArduinoJson::JsonObject &cfg) {
            liq->regist_tick_cb("search_c", this);
        }
        int oninit(ArduinoJson::JsonObject &cfg, std::map<std::string, CommonService*> &deps) {
            this->server = (SearchService*)deps["server"];
        }

        int ontick() {
            SearchRequest request;
            request.set_dd(23.23);
            SearchResponse *res = server->search(&request);
            if (res) {
                printf("%s:%d ontick [%d]\n", __FILE__, __LINE__, res->num());
            } else {
                printf("%s:%d ontick [%d]\n", __FILE__, __LINE__, 0);
            }
            return 0;
        }

    private:
        SearchService *server;
    };

    extern "C" {
        CommonService* create_module() {
            return new SearchC();
        }
    }
}

