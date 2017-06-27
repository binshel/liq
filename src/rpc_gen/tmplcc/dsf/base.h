#ifndef DSF_COMMON_H_
#define DSF_COMMON_H_

#include <stdint.h>
#include <string>
#include <google/protobuf/io/coded_stream.h>


class CommonSkeleton;

extern CommonSkeleton *skeleton;


class CommonService {
protected:
    std::string name;
public:
    virtual int _load() {
        return 0;
    }
};


class CommonSkeleton {
protected:
    std::string name;
public:
    virtual int handle(const char *method, uint8_t *reqBuff, int reqLen, 
            uint8_t **resBuff, int *resLen) = 0;
    uint8_t *getMsgBuff(int len) {
        return new uint8_t[len];
    };
    void releaseMsgBuff(uint8_t *buff, int len) {
        delete buff;
    };
};

class CommonStub {
protected:
public:
    uint8_t *getMsgBuff(int len) {
        return new uint8_t[len];
    };
    void releaseMsgBuff(uint8_t *buff, int len) {
        delete buff;
    };

    void rpc(const char *method, uint8_t *reqBuff, int reqLen, uint8_t **resBuff, int *reslen) {
        skeleton->handle(method, reqBuff, reqLen, resBuff, reslen);
    }
};

typedef CommonService* (*fcnCreateService)(std::string &name);
typedef CommonStub* (*fcnCreateStub)(std::string &name);
typedef CommonSkeleton* (*fcnCreateSkeleton)(CommonService *service);

#endif
