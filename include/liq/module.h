/**
 * @file module.h
 * @brief 加载和管理模块文件
 */

#pragma once

#include <stdint.h>
#include <string>
#include <map>

#include "liq.h"
#include "logger.h"

namespace liq {

    class CommonService;

    typedef CommonService* (*fcn_create_service)();
    typedef void (*fcn_destory_service)(CommonService*);

    /**
     * @brief 保存模块信息
     */
    class Module {
    public: 
        /**
         * @brief 生成一个模块
         * @param path 模块路径
         * @param handle 模块句柄
         */
        Module(const std::string &path, void *handle);

        /**
         * @brief 从模块创建服务
         */
        inline CommonService* create_service() {
            INFO << "create service" << file_path << EOL;
            if (!this->pcreate_service) {
                ERROR << "not symbol create_service" << file_path  << EOL;
                exit(-1);
            } else {
                return this->pcreate_service();
            }
        }

        /**
         * @brief 销毁服务
         */
        inline void destory_service(CommonService* service) {
            if (!this->pdestory_service) {
                INFO << "not symbol destory_service" << file_path << EOL;
            } else {
                INFO << "destory service" << file_path << EOL;
                this->pdestory_service(service);
            }
        }

    protected:
        std::string file_path;      ///< 模块路径
        fcn_create_service pcreate_service;     ///< 模块中创建服务的函数
        fcn_destory_service pdestory_service;   ///< 模块中销毁服务的函数
        void *handle;           // 模块句柄
    };


    /**
     * @brief 加载和管理模块
     */
    class ModuleManager {
    public:
        /**
         * @brief 加载指定模块
         * @param path 模块路径
         */
        Module *load(const std::string &path);

        /**
         * @brief 加载目录下的所有模块
         * @param path 目录路径
         * @param suf 模块的后缀名
         */
        void load_dir(const std::string &path, const std::string &suf = ".so");

    private:
        Module *load_file(const std::string &path);
    private:
        std::map<std::string, Module*> modules;     ///< 已加载的模块
    };
}
