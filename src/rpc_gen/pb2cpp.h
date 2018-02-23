#include <list>

#include <google/protobuf/descriptor.h>


using namespace google::protobuf;
namespace rpc_gen
{
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
        const char *name;
        const char *inputfile;
        const char *outdir;
        const char *idldir;
        const char *rpcdir;
        const char *package_path;
        std::list<char*> includes;
    };


    const FileDescriptor* parse_pb(std::list<char*> &includes, const char *inputfile);
    void gen_header(const char *inputfile, const char *outputdir, const FileDescriptor *file);
    void gen_stub(const char *inputfile, const char *outputdir, const FileDescriptor *file);
    void gen_skeleton(const char *inputfile, const char *outputdir, const FileDescriptor *file);
    void gen_idl(const Arguments &arg, const FileDescriptor *file);
}
