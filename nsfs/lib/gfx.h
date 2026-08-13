// gfx.h:
// Middleware module that interacts with software buffer and flushes it to
// hardware via oled.h

#ifndef GFX_H
#define GFX_H

#include "../config.h"
#include "oled.h"
#include <stdint.h>

typedef struct {
  uint8_t x;
  uint8_t y;
} gfx_pixel;

typedef struct {
  gfx_pixel p1;
  gfx_pixel p2;
} gfx_line;

typedef struct {
  gfx_pixel p1;
  gfx_pixel p2;
  uint8_t fill;
} gfx_rectangle;

typedef struct {
  gfx_pixel p1;
  gfx_pixel p2;
  gfx_pixel p3;
  uint8_t fill;
} gfx_triangle;

extern uint8_t oled_buffer[OLED_PAGES][OLED_SEG];

void gfx_draw_pixel(gfx_pixel pixel);
// Uses Bresenham's Line Algorithm
// TODO: Optimize drawing horizontal vs vertical lines
void gfx_draw_line(gfx_line line);

void gfx_draw_rectangle(gfx_rectangle rectangle);

// TODO: Filling the triangle
void gfx_draw_triangle(gfx_triangle triangle);

void gfx_draw_char(gfx_pixel pos, char c, uint8_t invert);
void gfx_draw_string(gfx_pixel pos, const char *s, uint8_t invert);

#endif
