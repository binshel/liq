#include <stdio.h>
#include "pb/test_s.h"

namespace test {
    class Search : public liq::ITickCB, public SearchService {
    public:
        int onload(LiqState *liq, ArduinoJson::JsonObject &cfg) {
            liq->regist_tick_cb("search_s", this);
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
                uint32_t i64, 
                int64_t ui32, 
                uint32_t ui64, 
                double dd, 
                float ff, 
                bool bb, 
                ENUM ee, 
                string* str) {
            printf("get dd %lf\n", dd);
            SearchResponse *res = new SearchResponse();
            res->set_num(this->count);
        }
    private:
        int32_t count;

    };

    extern "C" {
        CommonService* create_module() {
            return new Search();
        }
    }
}

