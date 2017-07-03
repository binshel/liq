#include <stdio.h>
#include "pb/test_s.h"

namespace test {
    class Search : public liq::ITickCB, public SearchService {
        int onload(LiqState *liq, ArduinoJson::JsonObject &cfg) {
            liq->regist_tick_cb("search_s", this);
        }
        int ontick() {
            printf("%s:%d ontick\n", __FILE__, __LINE__);
            return 0;
        }

    };

    extern "C" {
        CommonService* create_module() {
            return new Search();
        }
    }
}

