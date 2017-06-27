

#include "./test.h"

namespace test {

class Search: public SearchService {
public:
    Search(std::string &name) {
        this->name = name;
    }

    int _load() {
        return 0;
    }
    virtual inline SearchResponse *search(string* name, A* a, int32_t num, google::protobuf::RepeatedField<int32_t>* nums, google::protobuf::RepeatedPtrField<string>* strs, int32_t i32, uint32_t i64, int64_t ui32, uint32_t ui64, double dd, float ff, bool bb, ENUM ee, string* str) {
        SearchResponse *res = new SearchResponse();
        res->set_reply("reply");
        return res;
    }
};

extern "C" {
    CommonService *createService(std::string &name) {
        SearchService *service = new Search(name);
        return service;
    }
}
}
