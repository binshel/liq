#include <unistd.h>
#include <stdio.h>
#include <string>

#include "./command_line.h"
#include "liq.h"


void usage(int argc, char *argv[]) {
    printf(
        "Usage %s [option]\n"
        "options:\n"
        "  --cfgpath=cfgpath\n"
        , argv[0]);
    exit(-1);
}


int main(int argc, char *argv[]) {
    command_line::init(argc, argv);
    std::string *cfgpath = command_line::get_value("cfgpath")
    if (!cfgpath) {
        usage(argc, argv);
    }
    liq::LiqState *liq = liq::liq_init(cfgpath);

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
