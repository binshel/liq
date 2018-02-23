#include <string.h>

#define BIND_FUN_1(fun, args...)    std::bind(fun, ##args, std::placeholders::_1)
#define BIND_FUN_2(fun, args...)    std::bind(fun, ##args, std::placeholders::_1, std::placeholders::_2)
#define BIND_FUN_3(fun, args...)    std::bind(fun, ##args, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

inline void bin2hex(char *hex, const uint8_t *bin, int len)
{
    for (int i = 0; i < len; ++i) {
        sprintf(hex + i*2, "%02x", (unsigned char)bin[i]);
    }
    hex[len*2] = '\0';
}
static inline int atodigit(char a)
{
    if (a >= '0' && a <= '9') return a - '0';
    if (a >= 'a' && a <= 'f') return a - 'a' + 10;
    if (a >= 'A' && a <= 'F') return a - 'A' + 10;
    return 0;
}
inline void hex2bin(uint8_t *bin, const char *hex, int len)
{
    len = len / 2;
    for (uint8_t *i = bin, *j = (uint8_t*)hex; i < bin + len; ++i, j+=2) {
        *i = atodigit(*j) * 16 + atodigit(*(j+1));
    }
}

#define debug_hex(prompt, bin, len) \
{                                                       \
    char *tmp = (char*)calloc(1, len * 2 + 1);          \
    bin2hex(tmp, bin, len);                             \
    DEBUGF("%s %d:[%s]", prompt, len, tmp);             \
}                                
