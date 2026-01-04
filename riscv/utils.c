#include "utils.h"

void *ultoa(unsigned long value, unsigned char *buffer) {
    unsigned char *p = buffer;
    unsigned char *p1 = buffer;
    unsigned char tmp;

    // Generate digits in reverse order
    do {
        *p++ = (value % 10) + '0';
        value /= 10;
    } while (value);

    *p-- = '\0';

    // Reverse string
    while (p1 < p) {
        tmp = *p;
        *p-- = *p1;
        *p1++ = tmp;
    }
}
