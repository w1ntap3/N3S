#include "oled.h"
#include "../config.h"
#include "avr/io.h"
#include "util/delay.h"
#include <stdlib.h>

uint8_t oled_buffer[OLED_PAGES][OLED_SEG];

extern uint8_t spi_initialized;

void oled_cmd(uint8_t cmd) {
  OLED_DC_PORT &= ~(_BV(OLED_DC));
  spi_transfer(cmd);
}

void oled_data(uint8_t data) {
  OLED_DC_PORT |= _BV(OLED_DC);
  spi_transfer(data);
}

void oled_init(void) {
  // Initialize SPI, if already initialized, stop
  if (spi_initialized)
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

void oled_clear(void) {
  // Clear buffer
  for (int page = 0; page < OLED_PAGES; page++) {
    for (int seg = 0; seg < OLED_SEG; seg++) {
      oled_buffer[page][seg] = 0x00;
    }
  }

  // Horizontal addressing for convenience
  oled_cmd(OLED_START_ADDRESSING_MODE);
  oled_cmd(OLED_HORIZONTAL_ADDRESSING);

  // Reset to (0,0) with horizontal addressing
  oled_cmd(OLED_RESET_COLUMN_ADDRESS_HORIZONTAL);
  oled_cmd(0x00);
  oled_cmd(0x7F);

  oled_cmd(OLED_RESET_PAGE_ADDRESS_HORIZONTAL);
  oled_cmd(0x00);
  oled_cmd(0x07);

  // Clear GDDRAM
  for (int pixel = 0; pixel < OLED_PAGES * OLED_SEG; pixel++) {
    oled_data(0x00);
  }

  // Switch to regular addressing mode and enter data mode
  oled_cmd(OLED_START_ADDRESSING_MODE);
  oled_cmd(OLED_PAGE_ADDRESSING);

  OLED_DC_PORT |= _BV(OLED_DC);
}

void oled_flush(void) {
  // Horizontal addressing for convenience
  oled_cmd(OLED_START_ADDRESSING_MODE);
  oled_cmd(OLED_HORIZONTAL_ADDRESSING);

  // Reset to (0,0) with horizontal addressing
  oled_cmd(OLED_RESET_COLUMN_ADDRESS_HORIZONTAL);
  oled_cmd(0x00);
  oled_cmd(0x7F);

  oled_cmd(OLED_RESET_PAGE_ADDRESS_HORIZONTAL);
  oled_cmd(0x00);
  oled_cmd(0x07);

  // Flush the buffer into the GDDRAM
  for (int page = 0; page < OLED_PAGES; page++)
    for (int seg = 0; seg < OLED_SEG; seg++)
      oled_data(oled_buffer[page][seg]);

  // Switch to regular addressing mode and enter data mode
  oled_cmd(OLED_START_ADDRESSING_MODE);
  oled_cmd(OLED_PAGE_ADDRESSING);

  OLED_DC_PORT |= _BV(OLED_DC);
}
