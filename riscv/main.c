#include "desktop.h"
#include "utils.h"
#include "spi.h"

static unsigned long multix(unsigned long a, unsigned long b) {
    return (a + 5) * b;
}  

int main() {
    cls();
    
    unsigned char* content = read_flash();

    println(content);

    unsigned char temp[11];
    ultoa(multix(10, 15), temp);
    println(temp);

    println("Starting up TinyBasic Plus from RiscV...");

    setup();
    loop();

    return 0;
}
