#include <stdio.h>
#include "pb/test_s.h"

#include <liq/io.h>

namespace test {
    class Search : public liq::ITickCB, public SearchService {
    public:
        int onload(ArduinoJson::JsonObject &cfg) {
            liq::liq->regist_tick_cb("search_s", this);

            file.open("./cmake_install.cmake");
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
            
            char buff[1024];
            int32_t len = file.read(buff, 1023);

            res->set_reply(buff, len);
            res->set_num(this->count);

            printf("read [%s]\n", res->reply().c_str());
            
            return res;
        }
    private:
        int32_t count;
        liq::io::File file;

    };

    extern "C" {
        CommonService* create_module() {
            return new Search();
        }
    }
}

