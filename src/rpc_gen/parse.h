

namespace rpc_gen 
{
    bool pb_method_isoneway(const google::protobuf::MethodDescriptor *method);
    const std::string& pb_method_prepacker(const google::protobuf::MethodDescriptor *method);

    struct FieldType {
        std::string name;
        bool is_repeated;
        bool is_point;

        FieldType(const google::protobuf::FieldDescriptor *field);
    };
}
