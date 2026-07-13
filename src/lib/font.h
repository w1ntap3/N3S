#ifndef FONT_H
#define FONT_H

#include <stdint.h>
// File taken from https://github.com/efthymios-ks/AVR-SSD1306/

#define FONT_OFFSET 0x20
#define FONT_WIDTH 5
#define FONT_HEIGHT 8

extern const uint8_t font_array[];
#endif
