#include <msgpack.h>
#include "test.h"
#include "rpc_base.h"

#include "test.struct.h"


struct SearchResponse* stubSearch(char *name, struct A *a)
{

    struct SearchRequest request;
    struct SearchResponse *response;
    struct __rpc_Search_msg msg;

    msgpack_object deserialized;
    msgpack_zone mempool;
    msgpack_unpack_return ret;
    
    msgpack_packer pk;

    request.name = name;
    request.a = a;


    /* pack */
    msgpack_sbuffer_clear(rpc_sbuffer);
    msgpack_packer_init(&pk, rpc_sbuffer, msgpack_sbuffer_write);
    SearchRequest_encode(&request, &pk);

    /*  call  */
    __rpc_Search(rpc_sbuffer->data, rpc_sbuffer->size);

    /* unpack */
    msgpack_zone_init(&mempool, 2048);
    ret = msgpack_unpack(rpc_sbuffer->data, rpc_sbuffer->size, NULL, &mempool, &deserialized);
    if (ret != MSGPACK_UNPACK_SUCCESS) {
        return NULL;
    }
    response = SearchResponse_alloc();
    SearchResponse_decode(response, &deserialized);
    // msgpack_object_print(stdout, deserialized); puts("");  

    msgpack_zone_destroy(&mempool);
    
    return response;
}
