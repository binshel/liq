#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#include "dsf/base.h"
#include "test.h"
#include <map>

using namespace test;

// test.skeleton.so  test.so  test.stub.so
//
int main() {
    void *hpb = dlopen("./bin/test.pb.so", RTLD_LAZY | RTLD_GLOBAL);
    void *hpb2 = dlopen("./bin/test2.pb.so", RTLD_LAZY | RTLD_GLOBAL);
    void *hservice = dlopen("./bin/test.so", RTLD_LAZY | RTLD_LOCAL);
    void *hskeleton = dlopen("./bin/test.skeleton.so", RTLD_LAZY | RTLD_LOCAL);
    void *hstub = dlopen("./bin/test.stub.so", RTLD_LAZY | RTLD_LOCAL);

    fcnCreateService createService = (fcnCreateService)dlsym(hservice, "createService");
    fcnCreateStub createStub = (fcnCreateStub)dlsym(hstub, "createStub");
    fcnCreateSkeleton createSkeleton = (fcnCreateSkeleton)dlsym(hskeleton, "createSkeleton");

    std::string name = "test";
    CommonService *service = createService(name);
    CommonStub *stub = createStub(name);
    skeleton = createSkeleton(service);

    SearchService *search = (SearchService*)stub;
    std::string str("fwjoe");
    SearchResponse *res = search->search(&name, NULL, 1, NULL, NULL, 32, 64, 132, 164, 23.3, 24.3, false, (ENUM)-1, &str);
    delete res;

    return 0;
}
