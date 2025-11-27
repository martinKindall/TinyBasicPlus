#include "desktop.h"


static unsigned long rand_state = 1;  // 32-bit internal state

void srand(short int seed) {
    if (seed == 0) seed = 1; // avoid zero state
    rand_state = seed;
}

short int rand(void) {
    rand_state = rand_state * 1103515245 + 12345; // LCG
    return (short int)((rand_state >> 16) & 0x7FFF); // 0..32767
}
