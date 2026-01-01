#include "desktop.h"
#include "utils.h"
#include "spi.h"


int main() {
    cls();
    
    read_flash();

    unsigned char buf[11];

    for (int i = 0; i < 8; i++) {
        ultoa(read_flash_direct(i), buf);
        println(buf);
    }

    println("Starting up TinyBasic Plus from RiscV...");

    setup();
    loop();

    return 0;
}
