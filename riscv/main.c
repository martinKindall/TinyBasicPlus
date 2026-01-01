#include "desktop.h"
#include "utils.h"
#include "spi.h"


int main() {
    cls();
    
    unsigned char* content = read_flash();

    println(content);

    println("Starting up TinyBasic Plus from RiscV...");

    setup();
    loop();

    return 0;
}
