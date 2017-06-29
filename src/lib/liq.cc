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
        ArduinoJson::DynamicJsonBuffer json_buff(512);       
        std::ifstream ifs;
        ifs.open(cfgfile);
        ArduinoJson::JsonObject &root = json_buff.parse(ifs);

        ServiceManager::load_cfg(root);
    }
}
