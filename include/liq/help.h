/**
 * @file help.h
 * @brief 定义一些工具函数与宏
 */

#include <string.h>

/// @brief 为函数绑定一个参数
#define BIND_FUN_1(fun, args...)    std::bind(fun, ##args, std::placeholders::_1)
/// @brief 为函数绑定两个参数
#define BIND_FUN_2(fun, args...)    std::bind(fun, ##args, std::placeholders::_1, std::placeholders::_2)
/// @brief 为函数绑定三个参数
#define BIND_FUN_3(fun, args...)    std::bind(fun, ##args, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

/// @brief 转换二进制数据到16进程字符串
inline void bin2hex(char *hex, const uint8_t *bin, int len)
{
    for (int i = 0; i < len; ++i) {
        sprintf(hex + i*2, "%02x", (unsigned char)bin[i]);
    }
    hex[len*2] = '\0';
}

/// @brief 字符装换成16进程数字
static inline int atodigit(char a)
{
    if (a >= '0' && a <= '9') return a - '0';
    if (a >= 'a' && a <= 'f') return a - 'a' + 10;
    if (a >= 'A' && a <= 'F') return a - 'A' + 10;
    return 0;
}

/// @brief 转换16进程字符串到二进制数据
inline void hex2bin(uint8_t *bin, const char *hex, int len)
{
    len = len / 2;
    for (uint8_t *i = bin, *j = (uint8_t*)hex; i < bin + len; ++i, j+=2) {
        *i = atodigit(*j) * 16 + atodigit(*(j+1));
    }
}

/// @brief 已16进制字符串打印二进制数据
#define debug_hex(prompt, bin, len) \
{                                                       \
    char *tmp = (char*)calloc(1, len * 2 + 1);          \
    bin2hex(tmp, bin, len);                             \
    DEBUGF("%s %d:[%s]", prompt, len, tmp);             \
}                                
