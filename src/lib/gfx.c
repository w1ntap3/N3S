#include "gfx.h"
#include "../config.h"
#include "font.h"
#include "oled.h"
#include <stdlib.h>

static void gfx_draw_line_low_high(uint8_t x0, uint8_t y0, uint8_t x1,
                                   uint8_t y1, uint8_t lowhigh);
static void gfx_fill_rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

void gfx_draw_pixel(gfx_pixel pixel) {
  // Check boundaries
  if (pixel.x > 127 || pixel.y > 63)
    return;

  // Map Y to COM
  uint8_t page = (pixel.y - (pixel.y % 8)) / 8; // maps the y to a page 0-7
  uint8_t com_bit =
      (1 << (pixel.y -
             (page *
              8))); // binary 1 in a byte of where the row is at in that page

  uint8_t seg = pixel.x;

  // Set bit in buffer first
  oled_buffer[page][seg] |= com_bit;
}

// Both draw_line functions copied from Wikipedia
void gfx_draw_line(gfx_line line) {
  uint8_t x0 = line.p1.x;
  uint8_t y0 = line.p1.y;
  uint8_t x1 = line.p2.x;
  uint8_t y1 = line.p2.y;

  if (abs(y1 - y0) < abs(x1 - x0)) {
    if (x0 > x1)
      gfx_draw_line_low_high(x1, y1, x0, y0, 0);
    else
      gfx_draw_line_low_high(x0, y0, x1, y1, 0);
  } else {
    if (y0 > y1)
      gfx_draw_line_low_high(x1, y1, x0, y0, 1);
    else
      gfx_draw_line_low_high(x0, y0, x1, y1, 1);
  }
}

static void gfx_draw_line_low_high(uint8_t x0, uint8_t y0, uint8_t x1,
                                   uint8_t y1, uint8_t lowhigh) {
  int16_t dx, dy, D;

  if (lowhigh == 0) {
    dx = (int16_t)x1 - x0;
    dy = (int16_t)y1 - y0;
    int16_t yi = 1;
    if (dy < 0) {
      yi = -1;
      dy = -dy;
    }
    D = (2 * dy) - dx;
    int16_t y = y0;
    for (int16_t x = x0; x <= x1; x++) {
      gfx_pixel pix = {.x = (uint8_t)x, .y = (uint8_t)y};
      gfx_draw_pixel(pix);
      if (D > 0) {
        y += yi;
        D += 2 * (dy - dx);
      } else {
        D += 2 * dy;
      }
    }
  } else {
    dx = (int16_t)x1 - x0;
    dy = (int16_t)y1 - y0;
    int16_t xi = 1;
    if (dx < 0) {
      xi = -1;
      dx = -dx;
    }
    D = (2 * dx) - dy;
    int16_t x = x0;
    for (int16_t y = y0; y <= y1; y++) {
      gfx_pixel pix = {.x = (uint8_t)x, .y = (uint8_t)y};
      gfx_draw_pixel(pix);
      if (D > 0) {
        x += xi;
        D += 2 * (dx - dy);
      } else {
        D += 2 * dx;
      }
    }
  }
}

void gfx_draw_triangle(gfx_triangle triangle) {
  gfx_line AB = {.p1 = triangle.p1, .p2 = triangle.p2};
  gfx_line AC = {.p1 = triangle.p1, .p2 = triangle.p3};
  gfx_line BC = {.p1 = triangle.p2, .p2 = triangle.p3};

  gfx_draw_line(AB);
  gfx_draw_line(AC);
  gfx_draw_line(BC);
}

void gfx_draw_rectangle(gfx_rectangle rectangle) {
  gfx_pixel A_vertex = {.x = rectangle.p1.x, .y = rectangle.p1.y};
  gfx_pixel B_vertex = {.x = rectangle.p2.x, .y = rectangle.p1.y};
  gfx_pixel C_vertex = {.x = rectangle.p2.x, .y = rectangle.p2.y};
  gfx_pixel D_vertex = {.x = rectangle.p1.x, .y = rectangle.p2.y};

  gfx_line AB = {.p1 = A_vertex, .p2 = B_vertex};
  gfx_line BC = {.p1 = B_vertex, .p2 = C_vertex};
  gfx_line CD = {.p1 = C_vertex, .p2 = D_vertex};
  gfx_line DA = {.p1 = D_vertex, .p2 = A_vertex};

  gfx_draw_line(AB);
  gfx_draw_line(BC);
  gfx_draw_line(CD);
  gfx_draw_line(DA);

  if (rectangle.fill) {
    gfx_fill_rect(A_vertex.x + 1, A_vertex.y + 1, C_vertex.x - 1,
                  C_vertex.y - 1);
  }
}

static void gfx_fill_rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
  if (x0 > 127 || y0 > 63 || x1 < x0 || y1 < y0)
    return;

  for (uint8_t y = y0; y <= y1; y++) {
    for (uint8_t x = x0; x <= x1; x++) {
      uint8_t page = y / 8;
      oled_buffer[page][x] |= (1 << (y % 8));
    }
  }
}

void gfx_draw_char(gfx_pixel pos, char c, uint8_t invert) {
  // Out of bounds
  if (pos.x > 127 || pos.y > 63)
    return;
  // Character not in font
  if (c < 0x20 || c > 0x7F)
    return;

  uint8_t page = pos.y / 8;
  uint8_t y_offset = pos.y % 8;
  uint8_t seg = pos.x;
  uint16_t index = (c - FONT_OFFSET) * FONT_WIDTH;

  for (uint8_t col = 0; col < FONT_WIDTH; col++) {
    if (seg + col > 127)
      break;
    uint8_t data = invert ? ~font_array[index + col] : font_array[index + col];

    if (y_offset == 0) {
      oled_buffer[page][seg + col] = data;
    } else {
      oled_buffer[page][seg + col] &= ~((uint8_t)(0xFF << y_offset));
      oled_buffer[page][seg + col] |= (data << y_offset);

      if (page + 1 < OLED_PAGES) {
        oled_buffer[page + 1][seg + col] &=
            ~((uint8_t)(0xFF >> (8 - y_offset)));
        oled_buffer[page + 1][seg + col] |= (data >> (8 - y_offset));
      }
    }
  }
}

void gfx_draw_string(gfx_pixel pos, const char *s, uint8_t invert) {
  if (!s)
    return;

  gfx_pixel cursor = pos;
  while (*s) {
    gfx_draw_char(cursor, *s, invert);
    cursor.x += FONT_WIDTH + 1;
    s++;
  }
}
