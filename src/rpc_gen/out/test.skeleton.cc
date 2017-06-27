#include "dsf/base.h"
#include "./test.h"

using namespace std;

namespace test {
  class SearchSkeleton: public CommonSkeleton {
  private:
    SearchService *service;
  public:
    SearchSkeleton(CommonService *service) {
      this->service = (SearchService*)service;
    }

    virtual int handle(const char *method, uint8_t *reqBuff, int reqLen,uint8_t **resBuff, int *resLen) {
      if (strcmp(method, "search") == 0) {
        return search(reqBuff, reqLen, resBuff, resLen);
      } else {
        return -2;
      }
    }

    virtual int search(uint8_t *reqBuff, int reqLen,
         uint8_t **resBuff, int *resLen) {
      // 生成请求
      google::protobuf::io::CodedInputStream reqStream(reqBuff, reqLen);
      SearchRequest *req = new SearchRequest();
      req->MergePartialFromCodedStream(&reqStream);

      // 调用
      SearchResponse *res = service->search(req);
      delete req;

      // 打包结果
      if (res) {
        *resLen = res->ByteSize();
        *resBuff = getMsgBuff(*resLen);
        if (!(*resBuff)) {
          return -1;
        }
        res->SerializeWithCachedSizesToArray(*resBuff);
      } else {
        *resLen = 0;
      }
      return 0;
    }
  }; // class SearchSkeleton

  extern "C" {
    CommonSkeleton *createSkeleton(CommonService *service) {
      SearchSkeleton *skeleton = new SearchSkeleton(service);
      return skeleton;
    }
  }
} // namespace test

