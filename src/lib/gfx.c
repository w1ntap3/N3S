#include "gfx.h"
#include "oled.h"
#include "../config.h"
#include <stdlib.h>

static void gfx_draw_line_low_high(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t lowhigh);

void gfx_draw_pixel(gfx_pixel pixel)
{
    // Check boundaries
    if (pixel.x > 127 || pixel.y > 63){
        return;
    }

    // Map Y to COM
    uint8_t page = (pixel.y-(pixel.y%8))/8; // maps the y to a page 0-7
    uint8_t com_bit = (1 << (pixel.y-(page*8))); // binary 1 in a byte of where the row is at in that page
    
    uint8_t seg = pixel.x;

    // Set bit in buffer first
    oled_buffer[page][seg] |= com_bit;
}

// Both draw_line functions copied from Wikipedia
void gfx_draw_line(gfx_line line)
{
    uint8_t x0 = line.p1.x;
    uint8_t y0 = line.p1.y;
    uint8_t x1 = line.p2.x;
    uint8_t y1 = line.p2.y;

    if (abs(y1 - y0) < abs(x1 - x0)){
        if (x0 > x1)
            gfx_draw_line_low_high(x1, y1, x0, y0, 0);
        else
            gfx_draw_line_low_high(x0, y0, x1, y1, 0);
    } else
    {
        if (y0 > y1)
            gfx_draw_line_low_high(x1, y1, x0, y0, 1);
        else
            gfx_draw_line_low_high(x0, y0, x1, y1, 1);
    }
}

static void gfx_draw_line_low_high(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t lowhigh)
{
    int16_t dx, dy, D;

    if (lowhigh == 0) {
        dx = (int16_t)x1 - x0;
        dy = (int16_t)y1 - y0;
        int16_t yi = 1;
        if (dy < 0) { yi = -1; dy = -dy; }
        D = (2 * dy) - dx;
        int16_t y = y0;
        for (int16_t x = x0; x <= x1; x++) {
            gfx_pixel pix = {
                .x = (uint8_t)x,
                .y = (uint8_t)y
            };
            gfx_draw_pixel(pix);
            if (D > 0) { y += yi; D += 2 * (dy - dx); }
            else        {          D += 2 *  dy;        }
        }
    } else {
        dx = (int16_t)x1 - x0;
        dy = (int16_t)y1 - y0;
        int16_t xi = 1;
        if (dx < 0) { xi = -1; dx = -dx; }
        D = (2 * dx) - dy;
        int16_t x = x0;
        for (int16_t y = y0; y <= y1; y++) {
            gfx_pixel pix = {
                .x = (uint8_t)x,
                .y = (uint8_t)y
            };
            gfx_draw_pixel(pix);
            if (D > 0) { x += xi; D += 2 * (dx - dy); }
            else        {          D += 2 *  dx;        }
        }
    }
}

void gfx_draw_rectangle(gfx_rectangle rectangle)
{
}

void gfx_draw_triangle(gfx_triangle triangle)
{
}