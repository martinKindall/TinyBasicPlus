#include "desktop.h"
#include "utils.h"
#include "spi.h"


int main() {
    cls();
    
    unsigned char buf[11];
    ultoa(read_flash(), buf);
    println(buf);
    println("Starting up TinyBasic Plus from RiscV...");

    setup();
    loop();

    return 0;
}
