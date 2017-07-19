/*
 * =====================================================================================
 *
 *       Filename:  rpc_gen.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月12日 16时59分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/printer.h>


struct Env {
    std::string fileName;
    std::string fileShortName;
} env;

struct FieldType {
    std::string name;
    bool is_point;
    bool is_repeated;

    FieldType(const google::protobuf::FieldDescriptor *field)
    : is_repeated(false),  is_point(false) {
        const char *base_type_name = "";
        switch(field->cpp_type()) {
            case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
                base_type_name = "int32_t"; break;
            case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
                base_type_name = "uint32_t"; break;
            case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
                base_type_name = "int64_t"; break;
            case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
                base_type_name = "uint32_t"; break;
            case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
                base_type_name = "double"; break;
            case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
                base_type_name = "float"; break;
            case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
                base_type_name = "bool"; break;
            case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
                base_type_name = field->enum_type()->name().c_str();
                break;
            case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
                this->is_point = true;
                base_type_name = "string"; break;
            case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
                this->is_point = true;
                base_type_name = field->message_type()->name().c_str();
                break;
            defalut:
                base_type_name = field->cpp_type_name();
                break;
        }

        google::protobuf::FieldDescriptor::CppType type = field->cpp_type();
        std::ostringstream string_stream;
        if (field->is_repeated()) {
            if (type <= google::protobuf::FieldDescriptor::CPPTYPE_ENUM) {
                string_stream << "google::protobuf::RepeatedField<" << base_type_name << ">*";
            } else {
                string_stream << "google::protobuf::RepeatedPtrField<" << base_type_name << ">*";
            }
            this->is_repeated = true;
            this->is_point = true;
        } else {
            string_stream << base_type_name;
            if (this->is_point) {
                string_stream << "*";
            }
        }
        this->name = string_stream.str();
    }
};


typedef void fun_generate(const google::protobuf::FileDescriptor* file,
        google::protobuf::io::Printer &printer, std::string *error);


void generate_header(const google::protobuf::FileDescriptor* file,
        google::protobuf::io::Printer &printer, std::string *error)
{
    if (file->service_count() != 1) {
        error->append("must define one and only one service\n");
        return;
    }
    std::string fileNameUp = std::string(env.fileShortName);
    transform(fileNameUp.begin(), fileNameUp.end(), fileNameUp.begin(), toupper);


    printer.Print("#ifndef $H$_H_\n", "H", fileNameUp);
    printer.Print("#define $H$_H_\n\n", "H", fileNameUp);

    // include
    printer.Print("#include <google/protobuf/io/coded_stream.h>\n");
    printer.Print("#include \"liq/liq.h\"\n");
    printer.Print("#include \"liq/rpc.h\"\n");
    printer.Print("#include \"./$h$.pb.h\"\n\n", "h", env.fileShortName);

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
    printer.Print("class $s$Service: public CommonStub {\n", "s", service->name());
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
                "  return NULL;\n"
                "}\n"
                );
    }

    // ended
    printer.Outdent();
    printer.Print("}; // class $s$Service\n", "s", service->name());

    if (file->package().length() > 0) {
        printer.Outdent();
        printer.Print("} // namespace $space$\n", "space", file->package());
    }

    printer.Print("\n\n#endif //$H$_H_\n", "H", fileNameUp);
}
void generate_stub(const google::protobuf::FileDescriptor* file,
        google::protobuf::io::Printer &printer, std::string *error)

{
    // include
    printer.Print("#include \"liq/liq.h\"\n");
    printer.Print("#include \"liq/rpc.h\"\n");
    printer.Print("#include \"./$h$.h\"\n\n", "h", env.fileShortName);

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
    printer.Print("class $s$Stub: public $s$Service {\n", "s", service->name());
    printer.Print("public:\n");
    printer.Indent();

    // 构造函数
    /*
    printer.Print("$s$Stub(std::string &name) {\n"
            "  this->name = name;\n"
            "}\n\n", "s", service->name());
    */

    // class method
    for (int i = 0; i < service->method_count(); ++i) {
        const google::protobuf::MethodDescriptor *method = service->method(i);
        const google::protobuf::Descriptor *outputType = method->output_type();
        const google::protobuf::Descriptor *inputType = method->input_type();

        // inputType 做参数
        printer.Print(
                "virtual $out$ *$method$($input$ *req) {\n"
                "  // 调用\n"
                "  uint8_t *resBuff = NULL;\n"
                "  int resLen = 0;\n"
                "\n"
                "  rpc->call(\"$method$\", *req, &resBuff, &resLen);\n"
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
                "}\n",
            "out", outputType->name(), 
            "method", method->name(),
            "input", inputType->name());

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

        printer.Print("\n$output$ *res = this->$method$(&req);\n\n", "method", method->name(), "output", outputType->name());

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
            "  CommonService* create_module() {\n"
            "    $s$Stub *stub = new $s$Stub();\n"
            "    return stub;\n"
            "  }\n"
            "}\n", "s", service->name());

    if (file->package().length() > 0) {
        printer.Outdent();
        printer.Print("} // namespace $space$\n\n", "space", file->package());
    }

}

void  generate_skeleton(const google::protobuf::FileDescriptor* file,
        google::protobuf::io::Printer &printer, std::string *error)
{

    // include
    printer.Print("#include <google/protobuf/io/coded_stream.h>\n");
    printer.Print("#include \"liq/liq.h\"\n");
    printer.Print("#include \"liq/rpc.h\"\n");
    printer.Print("#include \"./$h$.h\"\n\n", "h", env.fileShortName);

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
            "  $s$Service *service;\n"
            , "s", service->name());
    printer.Print("public:\n");
    printer.Indent();

    // 构造函数
    /*
    printer.Print("$s$Skeleton(CommonService *service) {\n"
            "  this->service = ($s$Service*)service;\n"
            "}\n\n", "s", service->name());
    */

    // set_backend
    printer.Print("virtual void set_backend(CommonService *service) {\n"
            "  this->service = ($s$Service*)service;\n"
            "}\n\n", "s", service->name());
    // handle
    printer.Print("virtual google::protobuf::Message* handle(const char *method, uint8_t *reqBuff, int reqLen) {\n");
    printer.Indent();
    for (int i = 0; i < service->method_count(); ++i) {
        const google::protobuf::MethodDescriptor *method = service->method(i);
        const google::protobuf::Descriptor *outputType = method->output_type();
        const google::protobuf::Descriptor *inputType = method->input_type();
        
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
            "  CommonSkeleton *create_module() {\n"
            "    $s$Skeleton *skeleton = new $s$Skeleton();\n"
            "    return skeleton;\n"
            "  }\n"
            "}\n", "s", service->name());

    if (file->package().length() > 0) {
        printer.Outdent();
        printer.Print("} // namespace $space$\n\n", "space", file->package());
    }

}


void wirete_code (
        const google::protobuf::FileDescriptor* file,
        google::protobuf::compiler::GeneratorContext* generator_context, 
        std::string outFileName,
        std::string *error,
        fun_generate generate)
{
    google::protobuf::io::ZeroCopyOutputStream *output = generator_context->Open(outFileName);
    google::protobuf::io::Printer printer(output, '$');

    generate(file, printer, error);
}

class TestGrpcGenerator : public google::protobuf::compiler::CodeGenerator
{

    // 语法分析后生成代码
    virtual bool Generate(
            // 分析结果
            const google::protobuf::FileDescriptor* file,
            // 命令行参数
            const std::string& parameter,
            // 输出上下文
            google::protobuf::compiler::GeneratorContext* generator_context,
            // 出错后的错误信息
            std::string* error) const
    {
        env.fileName = file->name();
        env.fileShortName = env.fileName.substr(0, env.fileName.length() - strlen(".proto"));

        wirete_code(file, generator_context, env.fileShortName + ".h", error, generate_header);
        wirete_code(file, generator_context, env.fileShortName + ".stub.cc", error, generate_stub);
        wirete_code(file, generator_context, env.fileShortName + ".skeleton.cc", error, generate_skeleton);

        return true;
    }




};

int main(int argc, char *argv[])
{
    TestGrpcGenerator generator;
    return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}

