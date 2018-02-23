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
    void gen_stub(const char *inputfile, const char *outputdir, const FileDescriptor *file) 
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
        sprintf(name_buff, "%s/%s.stub.cc", outputdir, short_name.c_str());
        std::string output_file = std::string(name_buff);

        int fd = open(output_file.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (fd < 0) {
            fprintf(stderr, "can't open %s %s\n", output_file.c_str(), strerror(errno));
            exit(-2);
        } else {
            printf("stub output to %s(%d)\n", output_file.c_str(), fd);
        }
        google::protobuf::io::FileOutputStream output(fd);
        google::protobuf::io::Printer printer(&output, '$');

        // include
        printer.Print("#include \"liq/liq.h\"\n");
        printer.Print("#include \"liq/rpc.h\"\n");
        printer.Print("#include \"liq/service.h\"\n");
        printer.Print("#include \"liq/logger.h\"\n");
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
        printer.Print("class $s$Stub: public $s$ {\n", "s", service->name());
        printer.Print("public:\n");
        printer.Indent();

        // class method
        for (int i = 0; i < service->method_count(); ++i) {
            const google::protobuf::MethodDescriptor *method = service->method(i);
            const google::protobuf::Descriptor *outputType = method->output_type();
            const google::protobuf::Descriptor *inputType = method->input_type();

            std::map<std::string, std::string> variables;
            variables["method"] = method->name();
            variables["out"] = outputType->name();
            variables["input"] = inputType->name();
            variables["oneway"] = pb_method_isoneway(method) ? "RPC_ONEWAY": "RPC_TWOWAY";

            // inputType 做参数
            printer.Print(variables, 
                    "virtual $out$ *$method$_($input$ *req) {\n"
                    "  // 调用\n"
                    "  uint8_t *resBuff = NULL;\n"
                    "  int resLen = 0;\n"
                    "\n"
                    "  rpc->call($oneway$, \"$method$\", *req, &resBuff, &resLen);\n"
                    "\n"
                    "  // 获得结果\n"
                    "  if (resLen > 0) {\n"
                    "    google::protobuf::io::CodedInputStream resStream(resBuff, resLen);\n"
                    "    $out$ *res = new $out$();\n"
                    "    res->MergePartialFromCodedStream(&resStream);\n"
                    "    return res;\n"
                    "  } else {\n"
                    "    return NULL;\n"
                    "  }\n"
                    "}\n");
            printer.Print(variables, 
                    "virtual $out$ *$method$($input$ *req) {\n"
                    "  try {\n"
                    "    return this->$method$_(req);\n"
                    "  } catch (const char *e) {\n"
                    "    ERRORF(\"rpc call [%s] error [%s]\", \"call_sc_server\", e);\n"
                    "    throw e;\n"
                    "  }\n"
                    "}\n");

            // inputType 成员做参数
            printer.Print("virtual $out$ *$method$(", "out", outputType->name(), "method", method->name());
            for (int j = 0; j < inputType->field_count(); ++j) {
                const google::protobuf::FieldDescriptor *field = inputType->field(j);
                FieldType type(field);
                if (j == 0) {
                    printer.Print("$type$ $name$", "type", type.name, "name", std::string(field->name()));
                } else {
                    printer.Print(", $type$ $name$", "type", type.name, "name", std::string(field->name()));
                }
            }
            printer.Print(") {\n");
            printer.Indent();
            printer.Print("// 设置请求参数\n");
            printer.Print("$input$ req;\n", "input", inputType->name());
            for (int j = 0; j < inputType->field_count(); ++j) {
                const google::protobuf::FieldDescriptor *field = inputType->field(j);
                FieldType type(field);
                if (type.is_point) {
                    if (type.is_repeated) {
                        printer.Print(
                                "if ($name$) {\n"
                                "  for (int i = 0; i < $name$->size(); ++i) {\n"
                                "    req.add_$name$($name$->Get(i));\n"
                                "  }\n"
                                "}\n", "name", std::string(field->name()));
                    } else {
                        printer.Print("req.set_allocated_$name$($name$);\n", "name", std::string(field->name()));
                    }
                } else {
                    printer.Print("req.set_$name$($name$);\n", "name", std::string(field->name()));
                }
            }

            printer.Print("\n"
                    "$output$ *res;\n"
                    "try {\n"
                    "  res = this->$method$_(&req);\n"
                    "} catch (const char *e) {\n"
                    "  ERRORF(\"rpc call [%s] error [%s]\", \"$method$\", e);\n",
                    "method", method->name(), "output", outputType->name());

            for (int j = 0; j < inputType->field_count(); ++j) {
                const google::protobuf::FieldDescriptor *field = inputType->field(j);
                FieldType type(field);
                if (type.is_point && !type.is_repeated) {
                    printer.Print("  req.release_$name$();\n", "name", std::string(field->name()));
                }
            }

            printer.Print(
                    "  throw e;\n"
                    "}\n\n");

            printer.Print("// 返回\n");
            for (int j = 0; j < inputType->field_count(); ++j) {
                const google::protobuf::FieldDescriptor *field = inputType->field(j);
                FieldType type(field);
                if (type.is_point && !type.is_repeated) {
                    printer.Print("req.release_$name$();\n", "name", std::string(field->name()));
                }
            }
            printer.Print("return res;\n");
            printer.Outdent();
            printer.Print("}\n");
        }
        // ended
        printer.Outdent();
        printer.Print("}; // class $s$Stub\n\n", "s", service->name());

        // extern method
        printer.Print(
                "extern \"C\" {\n"
                "  CommonService* create_service() {\n"
                "    $s$Stub *stub = new $s$Stub();\n"
                "    return stub;\n"
                "  }\n"
                "}\n", "s", service->name());

        if (file->package().length() > 0) {
            printer.Outdent();
            printer.Print("} // namespace $space$\n\n", "space", file->package());
        }
    }
}
