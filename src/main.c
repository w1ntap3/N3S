#include "main.h"
#include "lib/oled.h"
#include "util/delay.h"
#include <stdint.h>
#include "config.h"
#include "lib/gfx.h"

const gfx_pixel test_pixel1 = {
        .x = 20,
        .y = 60
};

const gfx_pixel test_pixel2 = {
        .x = 64,
        .y = 20
};

const gfx_pixel test_pixel3 = {
        .x = 108,
        .y = 60
};

gfx_line test_line = {
        .p1 = test_pixel2,
        .p2 = test_pixel3
};

gfx_triangle test_triangle = {
        .p1 = test_pixel1,
        .p2 = test_pixel2,
        .p3 = test_pixel3,
        .fill = 1
};

gfx_rectangle test_rectangle = {
        .p1 = test_pixel1,
        .p2 = test_pixel2,
        .fill = 0
};

int main()
{
        ddr_setup();
        oled_init();

        while (1)
        {
                gfx_draw_pixel(test_pixel1);
                gfx_draw_pixel(test_pixel2);
                gfx_draw_pixel(test_pixel3);
                oled_flush();
                _delay_ms(500);
                oled_clear();
                gfx_draw_line(test_line);
                oled_flush();
                _delay_ms(500);
                oled_clear();
                gfx_draw_triangle(test_triangle);
                oled_flush();
                _delay_ms(500);
                oled_clear();
                gfx_draw_rectangle(test_rectangle);
                oled_flush();
                _delay_ms(500);
                oled_clear();
        }
        return 0;
}