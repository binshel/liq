#pragma once

const std::string LIQ_YIELD_TIMEOUT       = "yield timeout";


enum LiqErrNO
{
    LIQ_SUCCESS                 = 0,
    LIQ_ERR_IO_REGISTED         = 1000,
    LIQ_ERR_IO_UNREGISTED       = 1001,
    LIQ_ERR_IO_CANNT_REGIST     = 1002,
    LIQ_ERR_SOCK_NEW            = 1003,
    LIQ_ERR_SOCK_BIND           = 1004,
    LIQ_ERR_SOCK_LISTEN         = 1005,
    LIQ_ERR_FCNTL_ERROR         = 1006,
    LIQ_ERR_SOCK_OPT            = 1007,


    LIQ_ERR_END
};

