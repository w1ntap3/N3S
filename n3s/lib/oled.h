// AVR driver for SSD1306 4-wire SPI 0.96" with communication over spi.h

#ifndef OLED_H
#define OLED_H

#include "spi.h"
#include <stdint.h>

// Register values
#define OLED_SPCR                                                              \
  (SPI_SPCR_SPE | SPI_SPCR_DORD_MSB | SPI_SPCR_MSTR | SPI_SPCR_MODE0 |         \
   SPI_SPCR_DIV2)
#define OLED_SPSR (SPI_SPSR_SPI2X)

// Commands
#define OLED_CONTRAST_CONTROL 0x81
#define OLED_CONTRAST 255
#define OLED_SHOW_RAM 0xA4
#define OLED_IGNORE_RAM 0xA5
#define OLED_NORMAL 0xA6
#define OLED_INVERTED 0xA7
#define OLED_ON 0xAF
#define OLED_OFF 0xAE
#define OLED_CHARGE_PUMP_SETTING 0x8D
#define OLED_CHARGE_PUMP_ENABLE 0x14
#define OLED_START_ADDRESSING_MODE 0x20
#define OLED_HORIZONTAL_ADDRESSING 0b00
#define OLED_VERTICAL_ADDRESSING 0b01
#define OLED_PAGE_ADDRESSING 0b10
#define OLED_SET_PAGE_ADDRESS 0xB0
#define OLED_SET_LOWER_COLUMN 0x00
#define OLED_SET_HIGHER_COLUMN 0x10
#define OLED_RESET_PAGE_ADDRESS_HORIZONTAL 0x22
#define OLED_RESET_COLUMN_ADDRESS_HORIZONTAL 0x21
#define OLED_COM_PINS 0xDA
#define OLED_COM_PINS_VALUE 0x12

void oled_cmd(uint8_t cmd);
void oled_data(uint8_t data);

void oled_init(void);
void oled_clear(void);
void oled_flush(void);

#endif
