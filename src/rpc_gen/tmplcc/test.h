
#ifndef TEST_H_
#define TEST_H_


#include "dsf/base.h"
#include "./test.pb.h"

using namespace test;


class SearchService: public CommonService {
public:
    virtual test::SearchResponse *search(std::string *name, A *a, int32_t num) = 0;
};

#endif
