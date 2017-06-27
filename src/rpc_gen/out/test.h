#ifndef TEST_H_
#define TEST_H_

#include "dsf/base.h"
#include "./test.pb.h"

using namespace std;

namespace test {
  class SearchService: public CommonService {
  public:
    virtual inline SearchResponse *search(SearchRequest *req) {
      return search(req->mutable_name(), req->mutable_a(), req->num(), req->mutable_nums(), req->mutable_strs(), req->i32(), req->i64(), req->ui32(), req->ui64(), req->dd(), req->ff(), req->bb(), req->ee(), req->mutable_str());
    }
    virtual inline SearchResponse *search(string* name, A* a, int32_t num, google::protobuf::RepeatedField<int32_t>* nums, google::protobuf::RepeatedPtrField<string>* strs, int32_t i32, uint32_t i64, int64_t ui32, uint32_t ui64, double dd, float ff, bool bb, ENUM ee, string* str) {
      return NULL;
    }
  }; // class SearchService
} // namespace test


#endif //TEST_H_
