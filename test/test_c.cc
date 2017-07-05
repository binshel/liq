
#include <stdio.h>
#include "pb/test_s.h"
#include "pb/test_c.h"

namespace test {
    class SearchC : public liq::ITickCB, public SearchCService {
        int onload(LiqState *liq, ArduinoJson::JsonObject &cfg) {
            liq->regist_tick_cb("search_c", this);
        }
        int ontick() {
            printf("%s:%d ontick\n", __FILE__, __LINE__);
            return 0;
        }

    };

    extern "C" {
        CommonService* create_module() {
            return new SearchC();
        }
    }
}

