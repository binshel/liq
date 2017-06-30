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

using namespace liq;

/*
int main(int argc, const char *argv[])
{
    command_line::init(argc, argv);
    command_line::debug();

    module_loader::init();
    module_manager::init();

    return 0;
}
*/


extern "C" {
    void liq_init(const char *cfgfile) {
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

        ServiceManager::load_cfg(root);
    }
}
