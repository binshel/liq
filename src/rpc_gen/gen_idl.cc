#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include <algorithm>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/descriptor.pb.h>

#include "pb2cpp.h"
#include "parse.h"


namespace rpc_gen
{
    void gen_idl(const Arguments &arg, const FileDescriptor *file)
    {
        const char *inputfile = arg.inputfile;
        const char *outputdir = arg.idldir;
        char name_buff[512] = "";
        if (file->service_count() != 1) {
            printf("must define one and only one service\n");
            exit (-1);
        }
        std::string file_name = std::string(basename(inputfile));
        std::string short_name = file_name.substr(0, file_name.length() - strlen(".proto"));
        std::string file_name_up = std::string(short_name);
        std::transform(file_name_up.begin(), file_name_up.end(), file_name_up.begin(), ::toupper);
        sprintf(name_buff, "%s/%s.idl", outputdir, short_name.c_str());
        std::string output_file = std::string(name_buff);

        int fd = open(output_file.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (fd < 0) {
            fprintf(stderr, "can't open %s %s\n", output_file.c_str(), strerror(errno));
            exit(-2);
        } else {
            printf("idl output to %s(%d)\n", output_file.c_str(), fd);
        }
        google::protobuf::io::FileOutputStream output(fd);
        google::protobuf::io::Printer printer(&output, '$');

        // 开头
        const google::protobuf::ServiceDescriptor *service = file->service(0);
        printer.Print("{\n");
        printer.Indent();

        // 方法列表
        for (int i = 0; i < service->method_count(); ++i) {
            const google::protobuf::MethodDescriptor *method = service->method(i);
            bool oneway = pb_method_isoneway(method);
            printer.Print(
                    "\"$method$\": {\n"
                    "  \"oneway\": $oneway$\n"
                    "},\n",
                    "method", method->name(),
                    "oneway", oneway ? "true" : "false");
        }
        // pack_funs
        printer.Print("\"pack_funs\": {\n");
        printer.Indent();
        // pack_funs --> path
        printer.Print("\"path\": \"liq/liq_pack_funs\",\n");
        // pack_funs --> pb
        if (arg.package_path) {
            printer.Print("\"pb\": \"$package$/$short_name$\",\n",
                    "package", arg.package_path,
                    "short_name", short_name);
        } else if (file->package().length() > 0) {
            printer.Print("\"pb\": \"$package$/$short_name$\",\n",
                    "package", file->package().c_str(),
                    "short_name", short_name);
        } else {
            printer.Print("\"pb\": \"$short_name$\",\n",
                    "short_name", short_name);
        }
        // pack_funs --> methods
        printer.Print("\"methods\": {\n");
        printer.Indent();
        for (int i = 0; i < service->method_count(); ++i) {
            const google::protobuf::MethodDescriptor *method = service->method(i);
            printer.Print("\"$method$\": {\n", "method", method->name());
            printer.Indent();

            // prepacker
            const std::string &prepacker = pb_method_prepacker(method);
            if (prepacker.length() > 0) {
                printer.Print("\"prepacker\": \"$prepacker$\",\n", "prepacker", prepacker);
            }
            // in
            const google::protobuf::Descriptor *inputType = method->input_type();
            printer.Print("\"in\": {\n");
            printer.Indent();
            // in --> message
            printer.Print("\"message\": \"$input$\",\n", "input", inputType->full_name());

            printer.Print("\"fields\": [");
            for (int j = 0; j < inputType->field_count(); ++j) {
                const google::protobuf::FieldDescriptor *field = inputType->field(j);
                if (j == 0) {
                    printer.Print("\"$name$\"", "name", field->name());
                } else {
                    printer.Print(", \"$name$\"", "name", field->name());
                }
            }
            printer.Print("]\n");
            printer.Outdent();
            printer.Print("},\n");

            // out
            const google::protobuf::Descriptor *outputType = method->output_type();
            printer.Print("\"out\": {\n");
            printer.Indent();
            // in --> message
            printer.Print("\"message\": \"$output$\",\n", "output", outputType->full_name());

            printer.Print("\"fields\": [");
            for (int j = 0; j < outputType->field_count(); ++j) {
                const google::protobuf::FieldDescriptor *field = outputType->field(j);
                if (j == 0) {
                    printer.Print("\"$name$\"", "name", field->name());
                } else {
                    printer.Print(", \"$name$\"", "name", field->name());
                }
            }
            printer.Print("]\n");
            printer.Outdent();
            printer.Print("}\n");
            
            if (i == service->method_count() - 1) {
                printer.Outdent();
                printer.Print("}\n");
            } else {
                printer.Outdent();
                printer.Print("},\n");
            }
        }
        printer.Outdent();
        printer.Print("}\n"); // pack_funs --> methods
        printer.Print("}\n"); // pack_funs
        printer.Outdent();
        printer.Print("}\n"); // 结尾
    }
}
