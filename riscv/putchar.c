#include "desktop.h"

// --- Configuration Constants ---

// The base memory address of the framebuffer
#define FRAMEBUFFER_BASE ((volatile unsigned char *)0x401000)

static void scrollDown();

// The dimensions of the framebuffer
#define FRAMEBUFFER_COLS 80
#define FRAMEBUFFER_ROWS 30

// Static variables to maintain cursor position across function calls
static unsigned int s_cursor_col = 0;
static unsigned int s_cursor_row = 0;

// --- Function Implementation ---

/**
 * @brief Outputs a single character to the memory-mapped framebuffer.
 *
 * This function maintains a static internal cursor position (row, col).
 * - It handles newline characters ('\n') by moving the cursor to the
 * beginning of the next row.
 * - If the cursor exceeds the column width, it wraps to the next row.
 * - If the cursor exceeds the total row count, it wraps back to the
 * first row (row 0).
 *
 * @param c The character to write to the framebuffer.
 */
void putchar(unsigned char c) {
    // Pointer to the volatile memory-mapped I/O device
    volatile unsigned char *framebuffer = FRAMEBUFFER_BASE;

    // --- 1. Handle Character Input ---
    if (c == CR) {
        // noop
    } else if (c == NL) {
        // Newline character:
        // Reset column to 0 and advance to the next row
        s_cursor_col = 0;
        s_cursor_row++;
    } else {
        // All other characters:
        // Calculate the linear offset in the framebuffer
        unsigned int offset = (s_cursor_row * FRAMEBUFFER_COLS) + s_cursor_col;
        
        // Write the character to the memory-mapped device
        framebuffer[offset] = c;
        
        // Advance the column cursor
        s_cursor_col++;
    }

    // --- 2. Handle Column Wrap-around ---

    // If the column is now at or past the end of the line...
    if (s_cursor_col >= FRAMEBUFFER_COLS) {
        // ...reset column to 0 and advance to the next row
        s_cursor_col = 0;
        s_cursor_row++;
    }


    // If the row is now at or past the end of the screen...
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
