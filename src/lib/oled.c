#include "oled.h"
#include "avr/io.h"
#include "spi.h"
#include "../config.h"
#include "util/delay.h"
#include <stdlib.h>

uint8_t oled_buffer[OLED_PAGES][OLED_SEG];

void oled_init(void)
{
    // Initialize SPI, if already initialized, stop
    if (SPCR)
        spi_end();
    spi_init(OLED_SPCR, OLED_SPSR);

    // Turning on the OLED
    OLED_RES_PORT &= ~(_BV(OLED_RES));
    _delay_us(10);
    OLED_RES_PORT |= _BV(OLED_RES);
    OLED_DC_PORT &= ~(_BV(OLED_DC));

    // CS is pulled down only once, never meant to be HIGH ever
    OLED_CS_PORT &= ~(_BV(OLED_CS));

    // Initialization commands
    spi_transfer(OLED_OFF);
    spi_transfer(OLED_CONTRAST_CONTROL);
    spi_transfer(OLED_CONTRAST);
    spi_transfer(OLED_SHOW_RAM);
    spi_transfer(OLED_NORMAL);
    spi_transfer(OLED_CHARGE_PUMP_SETTING);
    spi_transfer(OLED_CHARGE_PUMP_ENABLE);
    spi_transfer(OLED_ON);
    spi_transfer(OLED_START_ADDRESSING_MODE);
    spi_transfer(OLED_PAGE_ADDRESSING);

    _delay_ms(100);

    oled_clear();

    // Data mode right after init
    OLED_DC_PORT |= _BV(OLED_DC);
}

void oled_clear(void)
{
    // Clear buffer
    for (int page = 0; page < OLED_PAGES; page++) {
        for (int seg = 0; seg < OLED_SEG; seg++) {
            oled_buffer[page][seg] = 0x00;
        }
    }

    // Horizontal addressing for convenient clearing
    OLED_DC_PORT &= ~(_BV(OLED_DC));
    spi_transfer(OLED_START_ADDRESSING_MODE);
    spi_transfer(OLED_HORIZONTAL_ADDRESSING);

    // Reset to (0,0) with horizontal addressing
    spi_transfer(OLED_RESET_COLUMN_ADDRESS_HORIZONTAL);
    spi_transfer(0x00);
    spi_transfer(0x7F);

    spi_transfer(OLED_RESET_PAGE_ADDRESS_HORIZONTAL);
    spi_transfer(0x00);
    spi_transfer(0x07);

    // Clear GDDRAM
    OLED_DC_PORT |= _BV(OLED_DC);
    for (int com = 0; com < OLED_PAGES * OLED_SEG; com++) {
        spi_transfer(0x00);
    }

    // Switch to regular addressing mode and enter data mode
    OLED_DC_PORT &= ~(_BV(OLED_DC));
    spi_transfer(OLED_START_ADDRESSING_MODE);
    spi_transfer(OLED_PAGE_ADDRESSING);
    OLED_DC_PORT |= _BV(OLED_DC);
}

oled_status oled_draw_pixel(oled_pixel pixel)
{
    // Check boundaries
    if (pixel.x > 127 || pixel.y > 63){
        return OLED_ERROR;
    }

    // Map Y to COM
    uint8_t page = (pixel.y-(pixel.y%8))/8; // maps the y to a page 0-7
    uint8_t com_bit = _BV(pixel.y-(page*8)); // binary 1 in a byte of where the row is at in that page
    
    uint8_t seg = pixel.x;

    // Position the cursor
    OLED_DC_PORT &= ~(_BV(OLED_DC));
    spi_transfer(OLED_SET_PAGE_ADDRESS | page);
    spi_transfer(OLED_SET_LOWER_COLUMN | (seg & 0x0F));
    spi_transfer(OLED_SET_HIGHER_COLUMN | (seg >> 4));

    // Set bit in buffer first
    oled_buffer[page][seg] |= com_bit;

    // Set bit in hardware
    OLED_DC_PORT |= _BV(OLED_DC);
    spi_transfer(oled_buffer[page][seg]);

    return OLED_OK;
}

// Both draw_line functions copied from Wikipedia
void oled_draw_line(oled_line line)
{
    uint8_t x0 = line.p1.x;
    uint8_t y0 = line.p1.y;
    uint8_t x1 = line.p2.x;
    uint8_t y1 = line.p2.y;

    if (abs(y1 - y0) < abs(x1 - x0)){
        if (x0 > x1)
            oled_draw_line_low_high(x1, y1, x0, y0, 0);
        else
            oled_draw_line_low_high(x0, y0, x1, y1, 0);
    } else
    {
        if (y0 > y1)
            oled_draw_line_low_high(x1, y1, x0, y0, 1);
        else
            oled_draw_line_low_high(x0, y0, x1, y1, 1);
    }
}

static void oled_draw_line_low_high(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t lowhigh)
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
            oled_pixel pix = {
                .x = (uint8_t)x,
                .y = (uint8_t)y
            };
            oled_draw_pixel(pix);
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
            oled_pixel pix = {
                .x = (uint8_t)x,
                .y = (uint8_t)y
            };
            oled_draw_pixel(pix);
            if (D > 0) { x += xi; D += 2 * (dx - dy); }
            else        {          D += 2 *  dx;        }
        }
    }
}

void oled_draw_rectangle(oled_rectangle rectangle)
{
}

void oled_draw_triangle(oled_triangle triangle)
{
}
