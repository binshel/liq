#include <unistd.h>
#include <iostream>
#include <string>

#include "liq.h"


int main() {
    
    liq::LiqState *liq = liq::liq_init("../test/test_s.cfg");

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
