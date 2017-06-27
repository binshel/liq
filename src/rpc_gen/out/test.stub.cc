#include "dsf/base.h"
#include "./test.h"

using namespace std;

namespace test {

  class SearchStub: public CommonStub, public SearchService {
  public:
    SearchStub(std::string &name) {
      this->name = name;
    }

    virtual SearchResponse *search(SearchRequest *req) {
      // 调用
      int reqLen = req->ByteSize();
      uint8_t *reqBuff = getMsgBuff(reqLen);
      uint8_t *resBuff = NULL;
      int resLen = 0;

      if (!reqBuff) {
        // TODO 错误处理
        return NULL;
      } else {
        req->SerializeWithCachedSizesToArray(reqBuff);
      }
      rpc("search", reqBuff, reqLen, &resBuff, &resLen);
      releaseMsgBuff(reqBuff, reqLen);

      // 获得结果
      if (resLen > 0) {
        google::protobuf::io::CodedInputStream resStream(resBuff, resLen);
        SearchResponse *res = new SearchResponse();
        res->MergePartialFromCodedStream(&resStream);
        return res;
      } else {
        return NULL;
      }
    }
    virtual SearchResponse *search(string* name, A* a, int32_t num, google::protobuf::RepeatedField<int32_t>* nums, google::protobuf::RepeatedPtrField<string>* strs, int32_t i32, uint32_t i64, int64_t ui32, uint32_t ui64, double dd, float ff, bool bb, ENUM ee, string* str) {
      // 设置请求参数
      SearchRequest req;
      req.set_allocated_name(name);
      req.set_allocated_a(a);
      req.set_num(num);
      if (nums) {
        for (int i = 0; i < nums->size(); ++i) {
          req.add_nums(nums->Get(i));
        }
      }
      if (strs) {
        for (int i = 0; i < strs->size(); ++i) {
          req.add_strs(strs->Get(i));
        }
      }
      req.set_i32(i32);
      req.set_i64(i64);
      req.set_ui32(ui32);
      req.set_ui64(ui64);
      req.set_dd(dd);
      req.set_ff(ff);
      req.set_bb(bb);
      req.set_ee(ee);
      req.set_allocated_str(str);

      SearchResponse *res = this->search(&req);

      // 返回
      req.release_name();
      req.release_a();
      req.release_str();
      return res;
    }
  }; // class SearchStub

  extern "C" {
    CommonStub *createStub(std::string &name) {
      SearchStub *stub = new SearchStub(name);
      return stub;
    }
  }
} // namespace test

