#include "desktop.h"


int main() {
    cls();
    println("Starting up TinyBasic Plus from RiscV...");

    setup();
    loop();

    return 0;
}
