#include <msgpack.h>
#include "test.h"
#include "rpc_base.h"

#include "test.struct.h"
// rpc Search
int __rpc_Search (char *data, int size)
{
    struct SearchRequest *request;
    struct SearchResponse *response;

    msgpack_object deserialized;
    msgpack_zone mempool;
    msgpack_unpack_return ret;
    
    msgpack_packer pk;

    /* unpack */
    msgpack_zone_init(&mempool, 2048);
    ret = msgpack_unpack(data, size, NULL, &mempool, &deserialized);
    if (ret != MSGPACK_UNPACK_SUCCESS) {
        return -1;
    }
    // msgpack_object_print(stdout, deserialized); puts("");  
    request = SearchRequest_alloc();
    SearchRequest_decode(request, &deserialized);

    response = Search(request->name, request->a);

    /* pack */
    msgpack_sbuffer_clear(rpc_sbuffer);
    msgpack_packer_init(&pk, rpc_sbuffer, msgpack_sbuffer_write);
    SearchResponse_encode(response, &pk);

    SearchRequest_free(request);
    SearchResponse_free(response);
    msgpack_zone_destroy(&mempool);
}

