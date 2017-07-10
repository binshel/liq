#include <unistd.h>
#include <stdio.h>
#include <string>

#include "./command_line.h"
#include "liq.h"


void usage(int argc, char *argv[]) {
    printf(
        "Usage %s [option]\n"
        "options:\n"
        "  --name node_name\n"
        "  --cfgpath cfgpath\n"
        , argv[0]);
    exit(-1);
}


int main(int argc, char *argv[]) {
    CommandLine::init(argc, argv);
    std::string *cfgpath = CommandLine::get_value("cfgpath");
    std::string *node_name = CommandLine::get_value("name");
    if (!cfgpath || !node_name) {
        usage(argc, argv);
    }
    liq::LiqState *liq = liq::liq_init(node_name->c_str(), cfgpath->c_str());

    for (;;) {
        int count = liq->ontick();
        if (count < 0) {
            break;      // 结束 
        } else if (count == 0){
            usleep(100);   // 暂停
        }
    }

    return 0;
}
