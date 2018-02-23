#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>

#include <list>

#include <google/protobuf/descriptor.h>

#include "pb2cpp.h"


static rpc_gen::Arguments arg;
static void help()
{
    printf("Usage: %s options\n", arg.name);
    printf("\t-I include proto diretory\n");
    printf("\t-o output diretory\n");
    printf("\t-i idl output diretory\n");
    printf("\t-r header, stub and skeleton output diretory\n");
    printf("\t-p package path\n");
    printf("\t-f proto file\n");
    exit (-1);
}

static char *get_path(const char *path)
{
    char *ab_path = realpath(path, NULL);
    if (!ab_path) {
        fprintf(stderr,"%s %s\n", path, strerror(errno));
        exit(-1);
    }
    return ab_path;
}

static void parse_arg(int argc, char *argv[])
{
    arg.name = argv[0];
    char c;
    while ((c = getopt(argc, argv, "p:f:I:o:i:r:")) != -1)
    {
        switch(c) {
        case 'I':
            arg.includes.push_front(get_path(optarg));
            break;
        case 'o':
            if (!arg.outdir) arg.outdir = get_path(optarg);
            else help();
            break;
        case 'i':
            if (!arg.idldir) arg.idldir = get_path(optarg);
            else help();
            break;
        case 'r':
            if (!arg.rpcdir) arg.rpcdir = get_path(optarg);
            else help();
            break;
        case 'f':
            if (!arg.inputfile){
                char *inputfile = get_path(optarg);
                arg.inputfile = basename(inputfile);
                arg.includes.push_front(dirname(strdup(inputfile)));
            } else {
                help();
            }
            break;
        case 'p':
        {
            char *begin = optarg;
            while (*begin && !isalnum (*begin)) ++begin;
            char *end = begin + strlen(begin);
            while (end >= begin && !isalnum(*end)) *end-- = '\0';
            if (*begin) arg.package_path = begin;
            else arg.package_path = NULL;
            break;
        }
        default:
            help();
            break;
        }
    }
    if (!arg.outdir){
        if (!arg.idldir) {
            printf("no idl output diretory\n");
            help();
        }
        if (!arg.rpcdir) {
            printf("no rpc output diretory\n");
            help();
        }
    }
    if (!arg.inputfile) {
        printf("no input file\n");
        help();
    }
    if (!arg.idldir) arg.idldir = arg.outdir;
    if (!arg.rpcdir) arg.rpcdir = arg.outdir;
}



using namespace rpc_gen;
using namespace google::protobuf;

int main(int argc, char *argv[]) 
{
    parse_arg(argc, argv);


    const FileDescriptor* file = rpc_gen::parse_pb(arg.includes, arg.inputfile);
    rpc_gen::gen_header(arg.inputfile, arg.rpcdir, file);
    rpc_gen::gen_stub(arg.inputfile, arg.rpcdir, file);
    rpc_gen::gen_skeleton(arg.inputfile, arg.rpcdir, file);
    rpc_gen::gen_idl(arg, file);
    return 0;
}
