#include "desktop.h"
#include "spi.h"

int main() {
    cls();
    
    unsigned long n_bytes = 256;
    unsigned char* content = read_flash(0, n_bytes);
    println_bytes(content, n_bytes);

    println("Starting up TinyBasic Plus from RiscV...");

    setup();
    loop();

    return 0;
}
