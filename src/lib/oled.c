#include "oled.h"
#include "avr/io.h"
#include "spi.h"
#include "../config.h"
#include "util/delay.h"

oled_status oled_init()
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

    OLED_CS_PORT |= _BV(OLED_CS);
    _delay_ms(100);
    return OLED_OK;
}