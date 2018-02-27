/**
 * @file error.h
 * @brief 定义错误码
 */


#pragma once

const std::string LIQ_YIELD_TIMEOUT       = "yield timeout";


/**
 * @brief liq 用到的错误码
 */
enum LiqErrNO
{
    LIQ_SUCCESS                 = 0,            ///< 无错误
    LIQ_ERR_IO_REGISTED         = 1000,         ///< 文件描述符已注册到 io 模块
    LIQ_ERR_IO_UNREGISTED       = 1001,         ///< 文件描述符未注册到 io 模块
    LIQ_ERR_IO_CANNT_REGIST     = 1002,         ///< 文件描述符不能注册，errno 保存了进一步信息
    LIQ_ERR_SOCK_NEW            = 1003,         ///< 创建 socket 错误
    LIQ_ERR_SOCK_BIND           = 1004,         ///< 绑定 socket 地址错误
    LIQ_ERR_SOCK_LISTEN         = 1005,         ///< socket 监听错误
    LIQ_ERR_FCNTL_ERROR         = 1006,         ///< 调用 fcntl 错误
    LIQ_ERR_SOCK_OPT            = 1007,         ///< 调用 setsockopt 错误


    LIQ_ERR_END
};

