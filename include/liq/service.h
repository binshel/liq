/**
 * @file service.h
 * @brief 创建和管理进程中的服务
 */

#pragma once

#include <jsoncpp/json/value.h>

#include "liq.h"

namespace liq {

    /**
     * @brief 定义服务接口
     */
    class CommonService {
        public:
            /**
             * @brief 服务被创建时调用，用于初始化服务自身
             */
            virtual int onload(Json::Value &cfg);

            /**
             * @brief 初始化服务状态，现在可以调用它的依赖项
             */
            virtual int oninit(Json::Value &cfg, std::map<std::string, CommonService*> &deps);
    };

    /**
     * @brief 定义 Stub 接口
     */
    class CommonStub : public CommonService {
        protected:
            RPC *rpc;       ///< Stub 对应的远程调用通道
        public:
            /**
             * @brief 设置使用的远程调用通道
             * @param rpc 远程调用通道
             */
            virtual void set_rpc(RPC *rpc);
    };

    /**
     * @brief 定义 skeleton 接口
     */
    class CommonSkeleton : public CommonService {
        public:
            /**
             * @brief 设置 skeleton 的后端服务
             * @param service 一个本地服务
             */
            virtual void set_backend(CommonService *service) = 0;

            /**
             * @brief 处理远程调用
             * @param method 调用的方法
             * @param reqBuff 调用参数缓冲区
             * @param reqLen 缓冲区长度
             * @return 服务的返回值
             */
            virtual google::protobuf::Message* handle(const char *method, uint8_t *reqBuff, int reqLen) = 0;
    };

    /**
     * @brief 服务的加载和管理类
     */
    class ServiceManager
    {
    public:
        ServiceManager();

        /**
         * @brief 加载配置，根据配置创建服务、stub、skeleton
         * @param cfg 配置信息
         */
        void load_cfg(Json::Value &cfg);

        /**
         * @brief 获得 skeleton 实例
         * @param name skeleton 名
         */
        CommonSkeleton* get_skeleton(const char *name);

    private:
        /**
         * @brief 配置加载完的后续处理
         * @param cfg 配置信息
         */
        void loaded_cfg(Json::Value &cfg);

        /**
         * @brief 创建配置中的 Stub
         * @param stub_root 配置中的 stub 信息
         */
        void add_stubs(Json::Value &stub_root);

        /**
         * @brief 创建配置中的服务
         * @param service_root 配置中的服务信息
         */
        void add_services(Json::Value &service_root);

        /**
         * @brief 创建配置中 Skeleton
         * @param accept_root 配置中的 skeleton 信息
         */
        void add_accepts(Json::Value &accept_root);

        std::map<std::string, CommonService*> services;     ///< 已创建的服务
        std::map<std::string, CommonSkeleton*> skeletons;   ///< 已创建的 skeleton

    };
}
