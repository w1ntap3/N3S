#include "main.h"
#include "lib/oled.h"
#include "util/delay.h"
#include <stdint.h>
#include "config.h"

int main()
{
        ddr_setup();
        _delay_ms(500);
        debug_func(oled_init(), OLED_OK);
        while (1)
        {
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

void debug_func(uint8_t actual, uint8_t expected)
{
        if (actual == expected)
        {
                PORTD |= _BV(DEBUG_GPIO);
                _delay_ms(100);
                PORTD &= ~(_BV(DEBUG_GPIO));
        }
        else
        {
                for (int i = 0; i < 10; i++)
                {
                        _delay_ms(30);
                        PORTD ^= _BV(DEBUG_GPIO);
                }
        }
}