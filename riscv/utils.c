#include "utils.h"

char *ultoa(unsigned long value, char *buffer) {
    char *p = buffer;
    char *p1 = buffer;
    char tmp;

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

    return buffer;
}
