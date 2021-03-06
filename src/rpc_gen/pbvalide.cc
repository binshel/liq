/**
 * @file pbvalide.cc
 * @brief 检查 proto 文件是否符合服务定义规范
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
    printf("\t-I include proto diretory\n");
    printf("\t-f proto file\n");
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
    while ((c = getopt(argc, argv, "f:I:o:i:r:")) != -1)
    {
        switch(c) {
        case 'I':
            arg.includes.push_front(get_path(optarg));
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
        default:
            help();
            break;
        }
    }
    if (!arg.inputfile) {
        printf("no input file\n");
        help();
    }
}


using namespace rpc_gen;
using namespace google::protobuf;

int main(int argc, char *argv[]) 
{
    parse_arg(argc, argv);

    // 解析输入文件
    const FileDescriptor* file = rpc_gen::parse_pb(arg.includes, arg.inputfile);

    // 文件只能拥有一个服务定义
    if (file->service_count() != 1) {
        return 1;
    } else {
        return 0;
    }
}
