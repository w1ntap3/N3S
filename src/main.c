#include "main.h"
#include "lib/oled.h"
#include "util/delay.h"
#include <stdint.h>
#include "config.h"

const oled_pixel test_pixel1 = {
        .x = 20,
        .y = 60
};

const oled_pixel test_pixel2 = {
        .x = 64,
        .y = 20
};

const oled_pixel test_pixel3 = {
        .x = 108,
        .y = 60
};

oled_line test_line = {
        .p1 = test_pixel2,
        .p2 = test_pixel3
};

oled_triangle test_triangle = {
        .p1 = test_pixel1,
        .p2 = test_pixel2,
        .p3 = test_pixel3,
        .fill = 1
};

oled_rectangle test_rectangle = {
        .p1 = test_pixel1,
        .p2 = test_pixel2,
        .fill = 0
};

int main()
{
        ddr_setup();
        oled_init();
        oled_clear();

        while (1)
        {
                oled_draw_pixel(test_pixel1);
                oled_draw_pixel(test_pixel2);
                oled_draw_pixel(test_pixel3);
                _delay_ms(500);
                oled_clear();
                oled_draw_line(test_line);
                _delay_ms(500);
                oled_clear();
                oled_draw_triangle(test_triangle);
                _delay_ms(500);
                oled_clear();
                oled_draw_rectangle(test_rectangle);
                _delay_ms(500);
                oled_clear();
        }
        return 0;
}

void ddr_setup(void)
{
        // clock_prescaler_off
        CLKPR = (1 << CLKPCE);
        CLKPR = 0;

        // enable gpio for debug
        DDRD |= _BV(DEBUG_GPIO);

        // OLED
        OLED_RES_DDR |= _BV(OLED_RES);
        OLED_DC_DDR |= _BV(OLED_DC);
        OLED_D0_DDR |= _BV(OLED_D0);
        OLED_D1_DDR |= _BV(OLED_D1);
        OLED_CS_DDR |= _BV(OLED_CS);
}

void debug_func(void)
{
        for (int i = 0; i < 10; i++)
        {
                _delay_ms(30);
                PORTD ^= _BV(DEBUG_GPIO);
        }
}