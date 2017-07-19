
#include <stdio.h>
#include "pb/test_s.h"
#include "pb/test_c.h"

namespace test {
    class SearchC : public liq::ITickCB, public SearchCService {
    public:
        int onload(ArduinoJson::JsonObject &cfg) {
            liq::liq->regist_tick_cb("search_c", this);
        }
        int oninit(ArduinoJson::JsonObject &cfg, std::map<std::string, CommonService*> &deps) {
            this->server = (SearchService*)deps["server"];
        }

        int ontick() {
            SearchRequest request;
            request.set_dd(23.23);
            SearchResponse *res = server->search(&request);
            if (res) {
                printf("%s:%d ontick [%d][%s]\n", __FILE__, __LINE__, res->num(), res->reply().c_str());
            } else {
                printf("%s:%d ontick [NULL]\n", __FILE__, __LINE__);
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

