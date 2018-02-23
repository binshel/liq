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
    void gen_header(const char *inputfile, const char *outputdir, const FileDescriptor *file)
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
        sprintf(name_buff, "%s/%s.h", outputdir, short_name.c_str());
        std::string output_file = std::string(name_buff);

        int fd = open(output_file.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (fd < 0) {
            fprintf(stderr, "can't open %s %s\n", output_file.c_str(), strerror(errno));
            exit(-2);
        } else {
            printf("head output to %s(%d)\n", output_file.c_str(), fd);
        }
        google::protobuf::io::FileOutputStream output(fd);
        google::protobuf::io::Printer printer(&output, '$');

        printer.Print("#ifndef $H$_H_\n", "H", file_name_up);
        printer.Print("#define $H$_H_\n\n", "H", file_name_up);

        // include
        printer.Print("#include <google/protobuf/io/coded_stream.h>\n");
        printer.Print("#include \"liq/liq.h\"\n");
        printer.Print("#include \"liq/rpc.h\"\n");
        printer.Print("#include \"liq/service.h\"\n");
        printer.Print("#include \"./$h$.pb.h\"\n\n", "h", short_name);

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
        printer.Print("class $s$: public CommonStub {\n", "s", service->name());
        printer.Print("public:\n");
        printer.Indent();

        // class method
        for (int i = 0; i < service->method_count(); ++i) {
            const google::protobuf::MethodDescriptor *method = service->method(i);
            const google::protobuf::Descriptor *outputType = method->output_type();
            const google::protobuf::Descriptor *inputType = method->input_type();

            // inputType 做参数
            printer.Print(
                    "virtual inline $out$ *$method$($input$ *req) {\n"
                    "  return $method$(",
                    "out", outputType->name(), 
                    "method", method->name(),
                    "input", inputType->name());
            printer.Indent();
            for (int j = 0; j < inputType->field_count(); ++j) {
                const google::protobuf::FieldDescriptor *field = inputType->field(j);
                FieldType type(field);
                std::string gap = (j == 0) ? "" : ", ";
                if (type.is_point) {
                    printer.Print("$gap$req->mutable_$name$()", "gap", gap, "name", std::string(field->name()));
                } else {
                    printer.Print("$gap$req->$name$()", "gap", gap, "name", std::string(field->name()));
                }
            }
            printer.Print(");\n");
            printer.Outdent();
            printer.Print("}\n");

            // inputType 成员做参数
            printer.Print("virtual inline $out$ *$method$(", "out", outputType->name(), "method", method->name());
            for (int j = 0; j < inputType->field_count(); ++j) {
                const google::protobuf::FieldDescriptor *field = inputType->field(j);
                FieldType type(field);
                if (j == 0) {
                    printer.Print("$type$ $name$", "type", type.name, "name", std::string(field->name()));
                } else {
                    printer.Print(", $type$ $name$", "type", type.name, "name", std::string(field->name()));
                }
            }
            printer.Print(
                    ") {\n"
                    "  throw \"call unimplement method \" __FILE__ \":$method$\";\n"
                    "  return NULL;\n"
                    "}\n", "method", method->name()
                    );
        }

        // ended
        printer.Outdent();
        printer.Print("}; // class $s$\n", "s", service->name());

        if (file->package().length() > 0) {
            printer.Outdent();
            printer.Print("} // namespace $space$\n", "space", file->package());
        }

        printer.Print("\n\n#endif //$H$_H_\n", "H", file_name_up);
        // output.Close();
    }
}
