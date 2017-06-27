

#include "./test.h"

class Search: public SearchService {
public:
    Search(std::string &name) {
        this->name = name;
    }

    int _load() {
        return 0;
    }

    SearchResponse *search(std::string *name, A *a, int32_t num) {
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
