#include <google/protobuf/descriptor.h>

using namespace google::protobuf;

namespace rpc_gen 
{
    /**
     * @brief 判断方法是否为 oneway 调用，由 proto 文件中函数的 oneway 选项指定，默认没有
     * @param method 方法描述符
     */
    bool pb_method_isoneway(const google::protobuf::MethodDescriptor *method);

    /**
     * @brief 函数的预打包函数，由 proto 文件中函数的 prepacker 选项指定，默认没有
     * @param method 方法描述符
     */
    const std::string& pb_method_prepacker(const google::protobuf::MethodDescriptor *method);

    /**
     * @brief 解析 proto 文件
     * @param includes 导入文件的查找路径
     * @param inputfile 输入（proto）文件路径
     * @return 成功解析后生成的文件描述符，如果解析错误，结束进程。
     */
    const FileDescriptor* parse_pb(std::list<char*> &includes, const char *inputfile);

    /**
     * @brief 保存 proto 字段的信息
     */
    struct FieldType {
        std::string name;   ///< 字段名
        bool is_repeated;   ///< 是否是 repeated 字段
        bool is_point;      ///< 是否是指针

        /**
         * @brief 从字段描述符生成字段信息
         * @param field proto 的字段描述符
         */
        FieldType(const google::protobuf::FieldDescriptor *field);
    };
}
