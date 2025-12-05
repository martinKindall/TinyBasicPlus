#include "stddef.h"

#define RISCV 1
#define boolean char
#define true 1
#define false 0
#define KEYBOARD_BUFFERED_DISABLED false

#define kRamSize 2*1024

#define CR	'\r'
#define NL	'\n'
#define BKSP 0x08


void cls();

void putchar(unsigned char c);

void print(const unsigned char *msg);

void println(const unsigned char *msg);

unsigned char getchar();

void srand(short int seed);

short int rand(void);

void setup(void);

void loop(void);
