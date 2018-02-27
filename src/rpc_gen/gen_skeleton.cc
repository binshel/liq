/**
 * @file gen_skeleton.h
 * @brief 生成服务的 skeleton 文件
 */


#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include <algorithm>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "pb2cpp.h"
#include "parse.h"


namespace rpc_gen
{
    /**
     * @brief 生成 skeleton 文件
     * @param inputfile proto 文件路径
     * @param outputdir 输出文件存放的目录
     * @param file proto 文件的解析结果
     */
    void gen_skeleton(const char *inputfile, const char *outputdir, const FileDescriptor *file) 
    {
        char name_buff[512] = "";
        if (file->service_count() != 1) {
            printf("must define one and only one service\n");
            exit (-1);
        }
        std::string file_name = std::string(basename(inputfile));
        std::string short_name = file_name.substr(0, file_name.length() - strlen(".proto"));
        std::string file_name_up = std::string(short_name);
        std::transform(file_name_up.begin(), file_name_up.end(), file_name_up.begin(), ::toupper);
        sprintf(name_buff, "%s/%s.skeleton.cc", outputdir, short_name.c_str());
        std::string output_file = std::string(name_buff);

        int fd = open(output_file.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (fd < 0) {
            fprintf(stderr, "can't open %s %s\n", output_file.c_str(), strerror(errno));
            exit(-2);
        } else {
            printf("skeleton output to %s(%d)\n", output_file.c_str(), fd);
        }
        google::protobuf::io::FileOutputStream output(fd);
        google::protobuf::io::Printer printer(&output, '$');

        // include
        printer.Print("#include <google/protobuf/io/coded_stream.h>\n");
        printer.Print("#include \"liq/liq.h\"\n");
        printer.Print("#include \"liq/rpc.h\"\n");
        printer.Print("#include \"./$h$.h\"\n\n", "h", short_name);

        // using namespace
        printer.Print("using namespace std;\n");
        printer.Print("using namespace liq;\n\n");

        // namespace
        if (file->package().length() > 0) {
            printer.Print("namespace $space$ {\n", "space", file->package());
            printer.Indent();
        }

        // class header
        const google::protobuf::ServiceDescriptor *service = file->service(0);
        printer.Print(
                "class $s$Skeleton: public CommonSkeleton {\n"
                "private:\n"
                "  $s$ *service;\n"
                , "s", service->name());
        printer.Print("public:\n");
        printer.Indent();

        // 构造函数
        /*
           printer.Print("$s$Skeleton(CommonService *service) {\n"
           "  this->service = ($s$*)service;\n"
           "}\n\n", "s", service->name());
           */

        // set_backend
        printer.Print("virtual void set_backend(CommonService *service) {\n"
                "  this->service = ($s$*)service;\n"
                "}\n\n", "s", service->name());
        // handle
        printer.Print("virtual google::protobuf::Message* handle(const char *method, uint8_t *reqBuff, int reqLen) {\n");
        printer.Indent();
        for (int i = 0; i < service->method_count(); ++i) {
            const google::protobuf::MethodDescriptor *method = service->method(i);
            // const google::protobuf::Descriptor *outputType = method->output_type();
            // const google::protobuf::Descriptor *inputType = method->input_type();

            if (i == 0) {
                printer.Print("if (strcmp(method, \"$method$\") == 0) {\n", "method", method->name());
            } else {
                printer.Print("} else if (strcmp(method, \"$method$\") == 0) {\n", "method", method->name());
            }
            printer.Print("  return $method$(reqBuff, reqLen);\n", "method", method->name());
        }
        if (service->method_count() > 0) {
            printer.Print(
                    "} else {\n"
                    "  return NULL;\n"
                    "}\n");
        } else {
            printer.Print("return NULL;\n");
        }
        printer.Outdent();
        printer.Print("}\n\n");

        // method
        for (int i = 0; i < service->method_count(); ++i) {
            const google::protobuf::MethodDescriptor *method = service->method(i);
            const google::protobuf::Descriptor *outputType = method->output_type();
            const google::protobuf::Descriptor *inputType = method->input_type();

            printer.Print(
                    "virtual google::protobuf::Message* $method$(uint8_t *reqBuff, int reqLen) {\n"
                    "  // 生成请求\n"
                    "  google::protobuf::io::CodedInputStream reqStream(reqBuff, reqLen);\n"
                    "  $input$ *req = new $input$();\n"
                    "  req->MergePartialFromCodedStream(&reqStream);\n"
                    "\n"
                    "  // 调用\n"
                    "  $out$ *res = service->$method$(req);\n"
                    "  delete req;\n"
                    "  return res;\n"
                    "}\n",
                    "out", outputType->name(), 
                    "method", method->name(),
                    "input", inputType->name());
        }
        // ended
        printer.Outdent();
        printer.Print("}; // class $s$Skeleton\n\n", "s", service->name());

        // extern method
        printer.Print(
                "extern \"C\" {\n"
                "  CommonSkeleton *create_service() {\n"
                "    $s$Skeleton *skeleton = new $s$Skeleton();\n"
                "    return skeleton;\n"
                "  }\n"
                "}\n", "s", service->name());

        if (file->package().length() > 0) {
            printer.Outdent();
            printer.Print("} // namespace $space$\n\n", "space", file->package());
        }
    }
}
