#include "desktop.h"

// The base memory address of the framebuffer
#define FRAMEBUFFER_BASE ((volatile unsigned char *)0x401000)

static void scrollDown();

#define FRAMEBUFFER_COLS 80
#define FRAMEBUFFER_ROWS 30

static unsigned int s_cursor_col = 0;
static unsigned int s_cursor_row = 0;

static boolean bufferedDisabled = KEYBOARD_BUFFERED_DISABLED;


/**
 * @brief Outputs a single character to the memory-mapped framebuffer.
 */
void putchar(unsigned char c) {
    volatile unsigned char *framebuffer = FRAMEBUFFER_BASE;

    if (c == CR) {
        return;
    } else if (c == BKSP) {
        if (bufferedDisabled) {
            return;
        }

        if (s_cursor_col > 0) {
            s_cursor_col--;                
            unsigned int offset = (s_cursor_row * FRAMEBUFFER_COLS) + s_cursor_col;
            framebuffer[offset] = 0;
        }

    } else if (c == NL) {
        s_cursor_col = 0;
        s_cursor_row++;
    } else {
        unsigned int offset = (s_cursor_row * FRAMEBUFFER_COLS) + s_cursor_col;
        
        framebuffer[offset] = c;
        
        s_cursor_col++;
    }

    if (s_cursor_col >= FRAMEBUFFER_COLS) {
        s_cursor_col = 0;
        s_cursor_row++;
    }


    if (s_cursor_row >= FRAMEBUFFER_ROWS) {
        scrollDown();

        s_cursor_row = FRAMEBUFFER_ROWS - 1;
    }
}

void print(const unsigned char *msg) {
    while(*msg) {
        putchar(*msg);
	    msg++;
	}
}

void println(const unsigned char *msg) {
    print(msg);
    putchar('\n');
}

void cls() {
    volatile unsigned char *framebuffer = FRAMEBUFFER_BASE;
    unsigned int offset;

    for (int i = 0; i < FRAMEBUFFER_ROWS; i++) {

        unsigned int offsetI = i * FRAMEBUFFER_COLS; 

        for (int j = 0; j < FRAMEBUFFER_COLS; j++) {
            offset = offsetI + j;
            framebuffer[offset] = 0;
        }
    }

    s_cursor_row = 0;
    s_cursor_col = 0;
}

static void scrollDown() {
    volatile unsigned char *framebuffer = FRAMEBUFFER_BASE;
    unsigned int offset;
    unsigned int offsetPrev;

    for (int i = 1; i < FRAMEBUFFER_ROWS; i++) {

        unsigned int offsetI = i * FRAMEBUFFER_COLS; 
        unsigned int offsetIPrev = (i-1) * FRAMEBUFFER_COLS; 
        
        for (int j = 0; j < FRAMEBUFFER_COLS; j++) {
            offset = offsetI + j;
            offsetPrev = offsetIPrev + j;

            // It takes 2 cycles to read from the framebuffer
            // Hence the first read is just garbage.
            unsigned char _not_used = framebuffer[offset]; 
            framebuffer[offsetPrev] = framebuffer[offset];
        }
    }

    // clear the last row
    for (int j = 0; j < FRAMEBUFFER_COLS; j++) {
        offset = (FRAMEBUFFER_ROWS - 1) * FRAMEBUFFER_COLS + j;
        framebuffer[offset] = 0;
    }
}
