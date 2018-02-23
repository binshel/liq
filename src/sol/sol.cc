#include <unistd.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <jsoncpp/json/reader.h>

#include "./command_line.h"
#include "liq/liq.h"
#include "liq/service.h"
#include "liq/logger.h"


const char *default_cfg =
"{ \n"
"   \"services\" : { \n"
"       \"%s\" : {\n"
"           \"service\" : \"sol/sc_client\",\n"
"           \"cfg\" : {},\n"
"           \"deps\" : {\n"
"               \"sc_agent\" : \"sc_agent\"\n"
"           }\n"
"       }\n"
"   },\n"
"   \"stubs\" : {\n"
"       \"sc_agent\" : {\n"
"           \"idl\" : \"sc_agent\",\n"
"           \"shm_size\" : 1024000,\n"
"           \"dst\" : \"sc_agent\"\n"
"       }\n"
"   }\n"
"}";


void usage() {
    printf(
        "Usage %s [option]\n"
        "options:\n"
        "  --name node_name\n"
        "  --cfgpath cfgpath\n"
        "  -hassc\n"
        , CommandLine::get_name().c_str());
    exit(-1);
}


void read_config_default(Json::Value &cfgroot)
{
    std::string *node_name = CommandLine::get_value("name");
    char *json = (char*)malloc(sizeof(*json) * 1024);
    int len = snprintf(json, 1024, default_cfg, node_name->c_str());
    DEBUGF("json config: %s", json);

    Json::Reader reader;
    if (!reader.parse(json, json + len, cfgroot)) {
        DEBUG << "parse config error: " << reader.getFormattedErrorMessages() << EOL;
        exit(-1);
    }
    free(json); 
}
void read_config_file(Json::Value &cfgroot, std::string &cfgpath)
{
    Json::Reader reader;
    std::fstream fs_cfg; 
    fs_cfg.open(cfgpath, std::fstream::in | std::fstream::binary);
    if (!reader.parse(fs_cfg, cfgroot)) {
        DEBUG << "parse config error: " << reader.getFormattedErrorMessages() << EOL;
        exit(-1);
    }
    fs_cfg.close();
}
void read_config(Json::Value &cfgroot)
{
    std::string *cfgpath = CommandLine::get_value("cfgpath");
    if (cfgpath) {
        read_config_file(cfgroot, *cfgpath);
    } else if (CommandLine::has_option("hassc")) {
        read_config_default(cfgroot);
    } else {
        DEBUG << "no define config method: " << EOL;
        usage();
    }
}



int main(int argc, char *argv[]) {
    CommandLine::init(argc, argv);
    std::string *node_name = CommandLine::get_value("name");
    if (!node_name) {
        usage();
    }

    liq::Liq::instance().init(*node_name);
    Json::Value cfgroot;
    read_config(cfgroot);
    liq::Liq::instance().load_cfg(cfgroot);

    for (;;) {
        int count = liq::Liq::instance().ontick();
        if (count < 0) {
            break;      // 结束 
        } 
    }

    return 0;
}
