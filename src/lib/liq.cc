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

#include "liq.h"
#include "rpc.h"
#include "service.h"
#include "module.h"


namespace liq {

    int CommonService::onload(LiqState *liq, ArduinoJson::JsonObject &cfg) {
        return 0;
    }
    int CommonService::oninit(ArduinoJson::JsonObject &cfg, std::map<std::string, CommonService*> &deps) {
        return 0;
    }
    void CommonStub::set_rpc(RPC *rpc) {
        this->rpc = rpc;
    }

    ITickCB::ITickCB()
        : intick(false) {
    }
    void ITickCB::call(ITickCB *cb) {
        cb->intick = true;
        cb->ontick();
        cb->intick = false;
    }

    LiqState::LiqState(const char *node_name, const char *cfgfile) {
        this->cfgfile = cfgfile;
        ArduinoJson::DynamicJsonBuffer json_buff(5120);       
        std::fstream fs_cfg;
        fs_cfg.open(cfgfile, std::fstream::in | std::fstream::binary);

        ArduinoJson::JsonObject &root = json_buff.parse(fs_cfg);
        if (!root.success()) {
            printf("json parse error\n");
            exit(-1);
        }

        
        this->rpc_manager = new RPCManager(node_name, this);
        this->module_manager = new ModuleManager();
        this->thread_pool = new ThreadPool();
        this->service_manager = new ServiceManager(this, root);
    }
    int LiqState::ontick() {
        int count = 0;

        // rpc handle
        count += this->rpc_manager->ontick();

        // registed tick
        for (auto it_cb = tick_cbs.begin();
                it_cb != tick_cbs.end(); 
                ++it_cb) { 
            ITickCB *cb = it_cb->second;
            if (cb->intick) continue;
            ++count;
            thread_pool->spawn((ThreadBase::fun_enter)ITickCB::call, cb);
        }
        return count;
    }
    void LiqState::regist_tick_cb(const std::string &name, ITickCB *cb) {
        this->tick_cbs[name] = cb;
    }


    extern "C" {
        LiqState* liq_init(const char *node_name, const char *cfgfile) {
            return new LiqState(node_name, cfgfile);
        }
    }
}
