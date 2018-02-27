/**
 * @file pb2cpp.cc
 * @brief 根据 proto 文件生成 c++ 文件和 idl 文件
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>

#include <list>

#include <google/protobuf/descriptor.h>

#include "parse.h"
#include "pb2cpp.h"


static rpc_gen::Arguments arg;

/**
 * @brief 打印帮组信息并退出程序
 */
static void help()
{
    printf("Usage: %s options\n", arg.name);
    printf("\t-I include proto diretory\n");    // proto 文件的查找路径
    printf("\t-o output diretory\n");           // 输出文件的存放目录
    printf("\t-i idl output diretory\n");       // 单独指定 idl 文件的输出目录
    // 单独指定 c++ 文件的输出目录
    printf("\t-r header, stub and skeleton output diretory\n");
    printf("\t-p package path\n");  // 用于idl文件只是目录前缀
    printf("\t-f proto file\n");    // proto 输入文件
    exit (-1);
}

/**
 * @brief 把一个路径转换为绝对路径
 * @param path 被转换的路径
 * @return 绝对路径
 */
static char *get_path(const char *path)
{
    char *ab_path = realpath(path, NULL);
    if (!ab_path) {
        fprintf(stderr,"%s %s\n", path, strerror(errno));
        exit(-1);
    }
    return ab_path;
}

/**
 * @brief 解析命令行参数
 * @param argc 参数个数
 * @param argv 参数列表
 */
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

    // 解析输入文件
    const FileDescriptor* file = rpc_gen::parse_pb(arg.includes, arg.inputfile);
    // 生成头文件
    rpc_gen::gen_header(arg.inputfile, arg.rpcdir, file);
    // 生成 stub 文件
    rpc_gen::gen_stub(arg.inputfile, arg.rpcdir, file);
    // 生成 skeleton 文件
    rpc_gen::gen_skeleton(arg.inputfile, arg.rpcdir, file);
    // 生成 idl 文件
    rpc_gen::gen_idl(arg, file);
    return 0;
}
