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


namespace liq {
    int CommonService::onload(LiqState *liq, ArduinoJson::JsonObject &cfg) {
        return 0;
    }
    int CommonService::oninit(ArduinoJson::JsonObject &cfg, std::map<std::string, CommonService*> &deps) {
        return 0;
    }

    LiqState::LiqState(const char *cfgfile) {
        this->cfgfile = cfgfile;
        ArduinoJson::DynamicJsonBuffer json_buff(5120);       
        std::fstream fs_cfg;
        fs_cfg.open(cfgfile, std::fstream::in | std::fstream::binary);

        ArduinoJson::JsonObject &root = json_buff.parse(fs_cfg);
        if (!root.success()) {
            printf("json parse error\n");
            exit(-1);
        }
        for (auto it = root.begin(); it != root.end(); ++it) {
            printf("key: %s\n", it->key);
        }

        ServiceManager::load_cfg(this, root);
    }
    int LiqState::ontick() {
        int count = 0;
        for (auto cb = tick_cbs.begin(); cb != tick_cbs.end(); ++cb) {
            count += cb->second->ontick();
        }
        return count;
    }
    void LiqState::regist_tick_cb(const std::string &name, ITickCB *cb) {
        this->tick_cbs[name] = cb;
    }


    extern "C" {
        LiqState* liq_init(const char *cfgfile) {
            return new LiqState(cfgfile);
        }
    }
}
