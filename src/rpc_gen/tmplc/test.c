/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年09月09日 14时53分13秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "test.h"
#include "test.struct.h"


struct SearchResponse* Search(char *name, struct A *a)
{
    struct SearchResponse *response = SearchResponse_alloc();
    response->reply = (typeof(response->reply))malloc(256);
    sprintf(response->reply, "hello %s for %s", name, "xxx");

    return response;
}


int i;
int main() {
    struct A a;
    char *name = "rison";
    char reply[256];

    on_load();

    for (i = 0; i < 100 * 10000; ++i) {
    // for (i = 0; i < 5; ++i) {
        struct SearchResponse *response = stubSearch(name, &a);
        // printf("replay: [%s] name: [%s]\n", response->reply, name);
        SearchResponse_free(response);
    }
    

    return 0;
}
