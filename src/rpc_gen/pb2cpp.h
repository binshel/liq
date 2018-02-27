/**
 * @file pb2cpp.h
 * @brief 根据 proto 文件生成需要的 c++ 文件和 idl 文件
 */


#include <list>

#include <google/protobuf/descriptor.h>

using namespace google::protobuf;

namespace rpc_gen
{
    /**
     * @brief 存储命令行参数信息
     */
    struct Arguments
    {
        Arguments()
        {
            name = NULL;        
            inputfile = NULL;   
            outdir = NULL;      
            idldir = NULL;      
            rpcdir = NULL;      
            package_path = NULL;
        }
        const char *name;           ///< 命令名称 
        const char *inputfile;      ///< 输入（proto）文件路径
        const char *outdir;         ///< 文件输出目录 
        const char *idldir;         ///< 单独指定 idl 文件输出目录 
        const char *rpcdir;         ///< 单独指定 c++ 文件输出目录
        const char *package_path;   ///< 用于 idl 文件标记 pb 文件前缀
        std::list<char*> includes;  ///< 导入文件的查找路径
    };
 

    void gen_header(const char *inputfile, const char *outputdir, const FileDescriptor *file);
    void gen_stub(const char *inputfile, const char *outputdir, const FileDescriptor *file);
    void gen_skeleton(const char *inputfile, const char *outputdir, const FileDescriptor *file);
    void gen_idl(const Arguments &arg, const FileDescriptor *file);
}
