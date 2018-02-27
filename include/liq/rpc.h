/**
 * @file rpc.h
 * @brief 远程调用功能模块
 */


#pragma once

#include <google/protobuf/message.h>

#include "liq.h"
#include "thread_pool.h"
// #include "shm_channel.h"

class tagChannel;
typedef tagChannel *LPCHANNEL;

namespace liq {

    /**
     * @brief 远程调用消息类型
     **/
    enum RPC_TYPE {
        RPC_TWOWAY = 0,     ///< twoway 调用请求
        RPC_RETURN = 1,     ///< 返回结果
        RPC_ERROR  = 2,     ///< 对方出错
        RPC_ONEWAY = 3      ///< oneway 调用请求
    };      

    /**
     * @brief 进程间通信接口
     */
    class IPCBase 
    {
        public:
            /**
             * @brief 发送消息
             * @param data 发送的消息
             * @param len 消息长度
             */
            virtual int32_t send(const uint8_t *data, int32_t len) = 0;

            /**
             * @brief 接收消息
             * @param buff 接收消息的缓冲区
             */
            virtual int32_t recv(uint8_t *buff) = 0;
    };

    /**
     *  @brief 共享内存实现进程间通信接口
     */
    class IPCShm : public IPCBase 
    {
        public:
            IPCShm() = delete;
            /**
             * @param from 通信的发起端
             * @param remote 通信的目标端
             * @param channelsize 共享内存的大小
             */
            IPCShm(const std::string &from, const std::string &remote,
                    const int channelsize = 100*1000);

            virtual int32_t send(const uint8_t *data, int32_t len);
            virtual int32_t recv(uint8_t *buff);

        private:
            LPCHANNEL channel;  ///< 共享内存管道指针
    };


    /**
     * @brief 一条远程调用通道
     */
    class RPC 
    {
        public:
            /**
             * @brief 被调的参数
             */
            struct BecallArg {
                IPCBase *ipc;       ///< 通信实现
                uint8_t *data;      ///< 打包后的参数
                int32_t data_len;   ///< 包长度
            };
            /**
             * @brief 对方返回的数据
             */
            struct ReturnArg {
                int32_t ecode;      ///< 错误码
                uint8_t *data;      ///< 打包后的的返回值
                int32_t data_len;   ///< 包长度 
            };
        public:
            RPC() = delete;
            /**
             * @param name 通道名
             * @param ipc 通道使用的通信接口
             */
            RPC(const std::string &name, IPCBase *ipc);

            /**
             * @brief 发起远程调用
             * @param type 调用类型
             * @param 调用的方法名
             * @param message 调用参数
             * @param resBuff 存放打包后的返回值的缓冲区
             * @param resLen  打包后的返回值的长度
             */
            virtual void call(RPC_TYPE type, const char *method, google::protobuf::Message &message,  uint8_t **resBuff, int32_t *resLen);

        private:
            std::string name;       ///< 通道名 
            IPCBase *ipc;           ///< 使用的通信接口
            static int64_t last_id; ///< 记录调用 ID
    };



    /**
     * @breif 管理远程调用
     */
    class RPCManager 
    {
        public:
            RPCManager() = delete;
            RPCManager(const std::string &node_name);

            /**
             * @brief 创建一个远程调用通道
             * @param name 通道名
             * @pram remote 通道的目标端
             */
            RPC* create_rpc(const char *name, const char *remote);

            /**
             * @brief 创建通信接口
             * @param remote 目标端
             */
            IPCBase* create_ipc(const char *remote);

            /**
             * @brief 处理进程间通信消息
             * @param ipc 通信接口
             */
            int32_t handle_ipc(IPCBase *ipc);

            /**
             *  @brief 处理远程调用
             */
            int32_t ontick();

        private:
            /**
             * @brief 被远程调用时的回调
             * @param arg 调用的参数 
             */
            void on_becall(RPC::BecallArg *arg);    

        protected:
            std::string node_name;                  ///< 节点名
            std::map<std::string, IPCBase*> ipcs;   ///< 已创建的通信接口
    };


}
