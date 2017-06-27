#pragma once

#include <msgpack.h>
#include "test.h"
#include "rpc_base.h"

// message A
static void A_encode(struct A *A, struct msgpack_packer *pk)
{
    msgpack_pack_map(pk, 0);
}
static int A_decode(struct A *A, struct msgpack_object *object)
{
    int i;
    if (object->type != MSGPACK_OBJECT_MAP) {
        return -1;
    }
    msgpack_object_map *map = &object->via.map;

    return 0;
}

// message SearchRequest
static void SearchRequest_encode(struct SearchRequest *SearchRequest, struct msgpack_packer *pk)
{
    msgpack_pack_map(pk, 2);
    // name
    rpc_pack_str(pk, "name");
    rpc_pack_str(pk, SearchRequest->name);
    rpc_pack_str(pk, "a");
    if (SearchRequest->a) {
        A_encode(SearchRequest->a, pk);
    } else {
        msgpack_pack_nil(pk);
    }
}

static int SearchRequest_decode(struct SearchRequest *SearchRequest, struct msgpack_object *object)
{
    int i;
    if (object->type != MSGPACK_OBJECT_MAP) {
        return -1;
    }
    msgpack_object_map *map = &object->via.map;
    for (i = 0; i < map->size; ++i) {
        if (map->ptr[i].key.type != MSGPACK_OBJECT_RAW) {
            return -1;
        } 

        if (0 == strncmp(map->ptr[i].key.via.raw.ptr, "name", map->ptr[i].key.via.raw.size)) {
            if (map->ptr[i].val.type == MSGPACK_OBJECT_NIL) {
                SearchRequest->name = NULL;
            } else if (map->ptr[i].val.type == MSGPACK_OBJECT_RAW) {
                SearchRequest->name = 
                    strndup(map->ptr[i].val.via.raw.ptr, map->ptr[i].val.via.raw.size);
            } else {
                return -1;
            }
        } else if (0 == strncmp(map->ptr[i].key.via.raw.ptr, "a", map->ptr[i].key.via.raw.size)) {
            if (map->ptr[i].val.type == MSGPACK_OBJECT_NIL) {
                SearchRequest->a = NULL;
            } else {
                SearchRequest->a = A_alloc();
                int ret = A_decode(SearchRequest->a, &map->ptr[i].val);
                if (0 != ret ) {
                    return ret;
                }
            }
        }
    }
    return 0;
}

// message SearchResponse
static void SearchResponse_encode(struct SearchResponse *SearchResponse, struct msgpack_packer *pk)
{
    msgpack_pack_map(pk, 1);
    // name
    rpc_pack_str(pk, "reply");
    rpc_pack_str(pk, SearchResponse->reply);
}

static int SearchResponse_decode(struct SearchResponse *SearchResponse, struct msgpack_object *object)
{
    int i;
    if (object->type != MSGPACK_OBJECT_MAP) {
        return -1;
    }
    msgpack_object_map *map = &object->via.map;
    for (i = 0; i < map->size; ++i) {
        if (map->ptr[i].key.type != MSGPACK_OBJECT_RAW) {
            return -1;
        } 

        if (0 == strncmp(map->ptr[i].key.via.raw.ptr, "reply", map->ptr[i].key.via.raw.size)) {
            if (map->ptr[i].val.type == MSGPACK_OBJECT_NIL) {
                SearchResponse->reply = NULL;
            } else if (map->ptr[i].val.type == MSGPACK_OBJECT_RAW) {
                SearchResponse->reply = 
                    strndup(map->ptr[i].val.via.raw.ptr, map->ptr[i].val.via.raw.size);
            } else {
                return -1;
            }
        }
    }
    return 0;
}



// message __rpc_Search_msg
struct __rpc_Search_msg {
    struct SearchRequest *request;
    struct SearchResponse *response;
};

static struct __rpc_Search_msg *__rpc_Search_msg_alloc() 
{
    struct __rpc_Search_msg *__rpc_Search_msg = 
        (typeof(__rpc_Search_msg))calloc(1, sizeof(*__rpc_Search_msg));
    return __rpc_Search_msg;
}

static void __rpc_Search_msg_destory(struct __rpc_Search_msg *__rpc_Search_msg) 
{
    if (__rpc_Search_msg->request)  SearchRequest_free(__rpc_Search_msg->request);
    if ( __rpc_Search_msg->response) SearchResponse_free(__rpc_Search_msg->response);

    bzero(__rpc_Search_msg, sizeof(*__rpc_Search_msg));
}
static void __rpc_Search_msg_free(struct __rpc_Search_msg *__rpc_Search_msg) 
{
    __rpc_Search_msg_destory(__rpc_Search_msg);
    free(__rpc_Search_msg);
}

static void __rpc_Search_msg_encode(struct __rpc_Search_msg *__rpc_Search_msg, struct msgpack_packer *pk)
{
    msgpack_pack_map(pk, 2);
    // name
    rpc_pack_str(pk, "request");
    if (__rpc_Search_msg->request) {
        SearchRequest_encode(__rpc_Search_msg->request, pk);
    } else {
        msgpack_pack_nil(pk);
    }
    rpc_pack_str(pk, "response");
    if (__rpc_Search_msg->response) {
        SearchResponse_encode(__rpc_Search_msg->response, pk);
    } else {
        msgpack_pack_nil(pk);
    }
}

static int __rpc_Search_msg_decode(struct __rpc_Search_msg *__rpc_Search_msg, struct msgpack_object *object)
{
    int i;
    if (object->type != MSGPACK_OBJECT_MAP) {
        return -1;
    }
    msgpack_object_map *map = &object->via.map;
    for (i = 0; i < map->size; ++i) {
        if (map->ptr[i].key.type != MSGPACK_OBJECT_RAW) {
            return -1;
        } 

        if (0 == strncmp(map->ptr[i].key.via.raw.ptr, "request", map->ptr[i].key.via.raw.size)) {
            if (map->ptr[i].val.type == MSGPACK_OBJECT_NIL) {
                __rpc_Search_msg->request = NULL;
            } else {
                __rpc_Search_msg->request = SearchRequest_alloc();
                int ret = SearchRequest_decode(__rpc_Search_msg->request, &map->ptr[i].val);
                if (0 != ret ) {
                    return ret;
                }
            }
        } else if (0 == strncmp(map->ptr[i].key.via.raw.ptr, "response", map->ptr[i].key.via.raw.size)) {
            if (map->ptr[i].val.type == MSGPACK_OBJECT_NIL) {
                __rpc_Search_msg->response = NULL;
            } else {
                __rpc_Search_msg->response = SearchResponse_alloc();
                int ret = SearchResponse_decode(__rpc_Search_msg->response, &map->ptr[i].val);
                if (0 != ret ) {
                    return ret;
                }
            }
        } else {
            return -1;
        }
    }
    return 0;
}


