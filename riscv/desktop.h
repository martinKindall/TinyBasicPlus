#define NULL ( (void *) 0)
#define boolean char
#define true 1
#define false 0

#define kRamSize 2*1024


void putchar(unsigned char c);

void print(const unsigned char *msg);

void println(const unsigned char *msg);

unsigned char getchar();

void srand(short int seed);

short int rand(void);

void setup(void);

void loop(void);
