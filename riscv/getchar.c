#include "desktop.h"

#ifndef byte
  typedef unsigned char byte;
#endif

#define KEYBOARD_BASE ((volatile unsigned short *)0x408000)

// ASCII Special Codes
#define BKSP 0x08
#define TAB  0x09
#define ENTER 0x0D
#define ESC  0x1B

// Lookup table: Index = Scancode, Value = ASCII
// optimized for "Set 2" (Standard)
static const unsigned char scancode_map[256] = {
    // 00 - 07
    0,     0,     0,     0,     0,     0,     0,     0,   
    // 08 - 0F
    0,     0,     0,     0,     0,     TAB,   '`',   0,   
    // 10 - 17
    0,     0,     0,     0,     0,     'q',   '1',   0,   
    // 18 - 1F
    0,     0,     'y',   's',   'a',   'w',   '2',   0,   
    // 20 - 27
    0,     'c',   'x',   'd',   'e',   '4',   '3',   0,   
    // 28 - 2F
    0,     ' ',   'v',   'f',   't',   'r',   '5',   0,   
    // 30 - 37
    0,     'n',   'b',   'h',   'g',   'z',   '6',   0,   
    // 38 - 3F
    0,     0,     'm',   'j',   'u',   '7',   '8',   0,   
    // 40 - 47
    0,     ',',   'k',   'i',   'o',   '0',   '9',   0,   
    // 48 - 4F
    0,     '.',   '/',   'l',   ';',   'p',   '-',   0,   
    // 50 - 57
    0,     0,     '\'',  0,     '[',   '=',   0,     0,   
    // 58 - 5F
    0,     0,     ENTER, ']',    0,    '\\',  0,     0,   
    // 60 - 67
    0,     0,     0,     0,     0,     0,     BKSP,  0,   
    // 68 - 6F (Keypad 1, etc start appearing here)
    0,     '1',   0,     '4',   '7',   0,     0,     0,   
    // 70 - 77
    '0',   '.',   '2',   '5',   '6',   '8',   ESC,   0,   
    // 78 - 7F
    0,     '+',   '3',   '-',   '*',   '9',   0,     0,   
    
    // 80 - FF
    // The rest are usually extended codes, F-keys, or release codes.
    // Leaving them as 0 for this basic implementation.
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0
};

static const unsigned char scancode_map_shifted[256] = {
    // 00 - 07
    0,      0,      0,      0,      0,      0,      0,      0,    
    // 08 - 0F
    0,      0,      0,      0,      0,      TAB,    '~',    0,    
    // 10 - 17
    0,      0,      0,      0,      0,      'Q',    '!',    0,    
    // 18 - 1F
    0,      0,      'Y',    'S',    'A',    'W',    '"',    0,    
    // 20 - 27
    0,      'C',    'X',    'D',    'E',    '$',    '#',    0,    
    // 28 - 2F
    0,      ' ',    'V',    'F',    'T',    'R',    '%',    0,    
    // 30 - 37
    0,      'N',    'B',    'H',    'G',    'Z',    '^',    0,    
    // 38 - 3F
    0,      0,      'M',    'J',    'U',    '&',    '*',    0,    
    // 40 - 47
    0,      '<',    'K',    'I',    'O',    ')',    '(',    0,    
    // 48 - 4F
    0,      '>',    '?',    'L',    ':',    'P',    '_',    0,    
    // 50 - 57
    0,      0,      '"',    0,      '{',    '+',    0,      0,    
    // 58 - 5F
    0,      0,      ENTER,  '}',    0,      '|',    0,      0,    
    // 60 - 67
    0,      0,      0,      0,      0,      0,      BKSP,   0,    
    // 68 - 6F (Keypad: shifted typically results in the same character)
    0,      '1',    0,      '4',    '7',    0,      0,      0,    
    // 70 - 77
    '0',    '.',    '2',    '5',    '6',    '8',    ESC,    0,    
    // 78 - 7F
    0,      '+',    '3',    '-',    '*',    '9',    0,      0,    
    
    // 80 - FF
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0
};


unsigned char getchar() {
  volatile unsigned short *keyboard = KEYBOARD_BASE;

  unsigned short data;
  byte char_data;
  boolean on_shift;

  while (true) {
    data = keyboard[0];

    if (data > 0x1FF) {
      keyboard[0] = 0;
      char_data = (byte) (data & 0xFF);
      on_shift = (boolean) ((data & 0x100) >> 8);
      
      break;
    }
  }

  return on_shift ? scancode_map_shifted[char_data] : scancode_map[char_data];
}
