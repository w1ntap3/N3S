#include "config.h"
#include <avr/io.h>
#include <util/delay.h>

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