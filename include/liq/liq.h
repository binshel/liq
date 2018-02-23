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

    typedef std::function<void()> fun_tick_cb;

    class ServiceManager;
    class ThreadPool;
    class ModuleManager;
    class RPC;
    class RPCManager;
    class TimerManager;
    class LoggerManager;
    class IO;

    class Liq : public Sington<Liq> 
    {
        friend class Sington<Liq>;
            Liq();
            ~Liq();
        public:
            void init(const std::string& node_name);

            inline const std::string &node_name()
            {
                return _node_name;
            }
            void load_cfg(Json::Value &cfg);
            int ontick();
            void regist_tick_cb(const std::string &name, const fun_tick_cb &cb);

        private:
            inline void set_time() {
                clock_gettime(CLOCK_REALTIME, &this->now);
                localtime_r(&this->now.tv_sec, &this->tm);
            }

        public:
            struct timespec now;
            struct tm tm;

            LoggerManager * logger;
            IO* io;
            ModuleManager *module;
            ThreadPool *thread_pool;
            TimerManager * timer;
            RPCManager *rpc;
            ServiceManager *service;
        private:
            std::string _node_name;
            std::string cfgfile;
            std::map<std::string, fun_tick_cb> tick_cbs;
    };

}

