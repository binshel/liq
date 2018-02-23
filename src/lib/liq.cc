/*
 * =====================================================================================
 *
 *       Filename:  container.cc
 *
 *    Description:  
 *
 *
 *        Version:  1.0
 *        Created:  2016年09月18日 16时10分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <iostream>
#include <string>
#include <fstream>

#include <jsoncpp/json/value.h>

#include "liq/liq.h"
#include "liq/rpc.h"
#include "liq/service.h"
#include "liq/module.h"
#include "liq/io.h"
#include "liq/logger.h"
#include "liq/timer.h"
#include "liq/stacktrace_imp.h"



namespace liq {
    
    int CommonService::onload(Json::Value &cfg) {
        return 0;
    }
    int CommonService::oninit(Json::Value &cfg, std::map<std::string, CommonService*> &deps) {
        return 0;
    }
    void CommonStub::set_rpc(RPC *rpc) {
        this->rpc = rpc;
    }

    Liq::Liq() :
        logger(NULL),
        io(NULL),
        module(NULL),
        thread_pool(NULL),
        timer(NULL),
        rpc(NULL),
        service(NULL)
    {
    }
    void Liq::init(const std::string &node_name)
    {
        _node_name = node_name;
        this->set_time();
        this->logger= new LoggerManager(node_name);
        this->io= new IO();
        this->module= new ModuleManager();
        this->thread_pool = new ThreadPool();
        this->timer= new TimerManager();
        this->rpc= new RPCManager(node_name);
        this->service= new ServiceManager();

        // 加载 pb 库
        this->module->load("liq.pb.so");
    }
    Liq::~Liq()
    {
        if (logger) delete logger;
        if (io) delete io;
        if (module) delete module;
        if (thread_pool) delete thread_pool;
        if (timer) delete timer;
        if (rpc) delete rpc;
        if (service) delete service;
    }
    void Liq::load_cfg(Json::Value &cfg)
    {
        this->service->load_cfg(cfg);
    }

    int Liq::ontick() {
        int count = 0;
        this->set_time();

        // rpc handle
        count += this->rpc->ontick();
        // io
        count += this->io->ontick();
        // thread_pool
        count += this->thread_pool->ontick();
        // timer
        count += this->timer->ontick();
        // registed tick
        for (auto it_cb = tick_cbs.begin();
                it_cb != tick_cbs.end(); 
                ++it_cb) 
        {
            fun_tick_cb &cb = it_cb->second;
            thread_pool->spawn(cb);
        }
        this->logger->ontick();
        return count;
    }
    void Liq::regist_tick_cb(const std::string &name, const fun_tick_cb &cb) {
        this->tick_cbs[name] = cb;
    }

}
