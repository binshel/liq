#pragma once

struct A {
};

struct SearchRequest {
    char *name;
    struct A *a;
};

struct SearchResponse {
    char *reply;
    size_t size;
};



// message A
static struct A *A_alloc() 
{
    struct A *A = (typeof(A))calloc(1, sizeof(*A));
    return A;
}

static void A_destory(struct A *A) 
{
}
static void A_free(struct A *A) 
{
    A_destory(A);
    free(A);
}


// message SearchRequest
static struct SearchRequest *SearchRequest_alloc() 
{
    struct SearchRequest *SearchRequest = 
        (typeof(SearchRequest))calloc(1, sizeof(*SearchRequest));
    return SearchRequest;
}

static void SearchRequest_destory(struct SearchRequest *SearchRequest) 
{
    if( SearchRequest->name)  free(SearchRequest->name);
    if( SearchRequest->a)  A_free(SearchRequest->a);

    bzero(SearchRequest, sizeof(*SearchRequest));
}
static void SearchRequest_free(struct SearchRequest *SearchRequest) 
{
    SearchRequest_destory(SearchRequest);
    free(SearchRequest);
}

// message SearchResponse
static struct SearchResponse *SearchResponse_alloc() 
{
    struct SearchResponse *SearchResponse = 
        (typeof(SearchResponse))calloc(1, sizeof(*SearchResponse));
    return SearchResponse;
}

static void SearchResponse_destory(struct SearchResponse *SearchResponse) 
{
    if (SearchResponse->reply) free(SearchResponse->reply);

    bzero(SearchResponse, sizeof(*SearchResponse));
}
static void SearchResponse_free(struct SearchResponse *SearchResponse) 
{
    SearchResponse_destory(SearchResponse);
    free(SearchResponse);
}



int __rpc_Search(char *data, int size);
struct SearchResponse* stubSearch(char *name, struct A *a);
struct SearchResponse* Search(char *name, struct A *a);

