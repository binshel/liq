
#include "dsf/base.h"

#include "./test.h" 


class SearchSkeleton: public CommonSkeleton {
private:
    SearchService *service;
public:
    SearchSkeleton(CommonService *service) {
        this->service = (SearchService*)service;
    }

    virtual int handle(const char *method, uint8_t *reqBuff, int reqLen, 
            uint8_t **resBuff, int *resLen) {
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
        SearchResponse *res = service->search(req->mutable_name(), req->mutable_a(), req->num());

        // 打包结果
        delete req;
        *resLen = res->ByteSize();
        *resBuff = getMsgBuff(*resLen);
        if (!(*resBuff)) {
            return -1;
        }
        res->SerializeWithCachedSizesToArray(*resBuff);
        return 0;
    }
};

extern "C" {
    CommonSkeleton *createSkeleton(CommonService *service) {
        SearchSkeleton *skeleton = new SearchSkeleton(service);
        return skeleton;
    }

}
