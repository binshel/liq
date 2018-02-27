/**
 * @file liq.h
 * @brief liq 库的对外接口，组织各个模块协调工作
 */


#pragma once

#include <stdint.h>
#include <time.h>
#include <string>
#include <map>
#include <functional>

#include <jsoncpp/json/value.h>
#include <google/protobuf/message.h>

#include "config.h"
#include "error.h"
#include "sington.h"

namespace liq {

    /// @brief 底层模块帧回调类型
    typedef std::function<void()> fun_tick_cb;

    class ServiceManager;
    class ThreadPool;
    class ModuleManager;
    class RPC;
    class RPCManager;
    class TimerManager;
    class LoggerManager;
    class IO;

    /**
     * 组织底层模块，有序调用它们
     */
    class Liq : public Sington<Liq> 
    {
        friend class Sington<Liq>;
            Liq();
            ~Liq();
        public:
            /**
             * @brief 初始化 liq 库
             * @param node_name liq 库节点名
             */
            void init(const std::string& node_name);

            inline const std::string &node_name()
            {
                return _node_name;
            }

            /**
             * @brief 加载配置信息
             * @param cfg 配置信息
             */
            void load_cfg(Json::Value &cfg);

            /**
             * @brief 被调用来处理一次调度工作
             */
            int ontick();

            /**
             * @brief 注册被调度时，要回调的函数
             */
            void regist_tick_cb(const std::string &name, const fun_tick_cb &cb);

        private:
            /**
             * @brief 设置当前时间戳与日期
             */
            inline void set_time() {
                clock_gettime(CLOCK_REALTIME, &this->now);
                localtime_r(&this->now.tv_sec, &this->tm);
            }

        public:
            struct timespec now;        ///< 当前日志
            struct tm tm;               ///< 当前时间戳

            LoggerManager * logger;     ///< 日志模块指针
            IO* io;                     ///< IO 模块指针
            ModuleManager *module;      ///< 模块管理模块指针
            ThreadPool *thread_pool;    ///< 协程池模块指针
            TimerManager * timer;       ///< 定时器模块指针
            RPCManager *rpc;            ///< 远程调用管理模块指针
            ServiceManager *service;    ///< 服务管理模块指针
        private:
            std::string _node_name;     ///< 节点名 
            std::map<std::string, fun_tick_cb> tick_cbs;    ///< 已注册的调度回调函数
    };

}

