
#include "dsf/base.h"
#include "./test.h"


class SearchServiceStub : public CommonStub, public SearchService  {
public:
    SearchServiceStub(std::string &name) {
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
        google::protobuf::io::CodedInputStream resStream(resBuff, resLen);
        SearchResponse *res = new SearchResponse();
        res->MergePartialFromCodedStream(&resStream);
        
        return res;
    }
    virtual SearchResponse *search(std::string *name, A *a, int32_t num) {
        // 设置请求参数
        SearchRequest req;
        req.set_allocated_name(name);
        req.set_allocated_a(a);
        req.set_num(num);

        SearchResponse *res = this->search(&req);

        // 返回 
        req.release_name();
        req.release_a();
        return res;
    }
};

extern "C" {
    CommonStub *createStub(std::string &name) {
        SearchServiceStub *stub = new SearchServiceStub(name);
        return stub;
    }
}
