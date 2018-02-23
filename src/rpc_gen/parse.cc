#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <list>
#include <sstream>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/compiler/importer.h>

#include "parse.h"


using namespace google::protobuf;
using namespace google::protobuf::compiler;

namespace rpc_gen
{
    bool pb_method_isoneway(const google::protobuf::MethodDescriptor *method)
    {
        const google::protobuf::MethodOptions &opts = method->options();
        for (int j = 0; j < opts.unknown_fields().field_count(); ++j) {
            auto opt = opts.unknown_fields().field(j);
            if (opt.number() == 50001) {
                return opt.varint();
                break;
            }
        }
        return true;
    }
    const std::string& pb_method_prepacker(const google::protobuf::MethodDescriptor *method)
    {
        static const std::string empty = "";
        const google::protobuf::MethodOptions &opts = method->options();
        for (int j = 0; j < opts.unknown_fields().field_count(); ++j) {
            auto opt = opts.unknown_fields().field(j);
            if (opt.number() == 50002) {
                return opt.length_delimited();
            }
        }
        return empty;
    }


    FieldType::FieldType(const google::protobuf::FieldDescriptor *field)
        : is_repeated(false),  is_point(false) 
    {
        const char *base_type_name = "";
        switch(field->cpp_type()) {
            case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
                base_type_name = "int32_t"; break;
            case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
                base_type_name = "int64_t"; break;
            case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
                base_type_name = "uint32_t"; break;
            case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
                base_type_name = "uint64_t"; break;
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
            default:
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

    class ParseError : public MultiFileErrorCollector
    {
    public:
        ParseError()
        {
            has_error = false;
        }
        virtual void AddError(const string& filename, int line, int column, const string& message)
        {
            printf("%s:%d column(%d) %s\n", filename.c_str(), line, column, message.c_str());
            has_error = true;
        }
        bool has_error;
    };
    const FileDescriptor* parse_pb(std::list<char*> &includes, const char *inputfile)
    {
        ParseError error;
        DiskSourceTree tree;
        for (auto &include : includes) {
            tree.MapPath("", include);
        }
 
        Importer *importer = new Importer(&tree, &error);
        const FileDescriptor * file = importer->Import(inputfile);
        if (error.has_error) {
            exit(-1);
        }
        return file;
    }
}

