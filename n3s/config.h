// config.h:
// Board support for AVR MCUs
#ifndef CONFIG_H
#define CONFIG_H
#include "lib/font.h"
#include <avr/io.h>
// NSD

// SPI OLED pinout
#define OLED_D0 PB5 // D0 is SCLK in 4 wire SPI mode
#define OLED_D1 PB3 // D0 is SDIN in 4 wire SPI mode
#define OLED_RES PB1
#define OLED_DC PB0
#define OLED_CS PB2

#define OLED_D0_PORT PORTB
#define OLED_D1_PORT PORTB
#define OLED_RES_PORT PORTB
#define OLED_DC_PORT PORTB
#define OLED_CS_PORT PORTB

#define OLED_D0_DDR DDRB
#define OLED_D1_DDR DDRB
#define OLED_RES_DDR DDRB
#define OLED_DC_DDR DDRB
#define OLED_CS_DDR DDRB

// Screen resolution
#define OLED_SCREEN_WIDTH 128
#define OLED_SCREEN_HEIGHT 64
#define OLED_PAGES 8
#define OLED_SEG 128

// UI grid
#define CELL_WIDTH (OLED_SCREEN_WIDTH / 2)
#define CELL_HEIGHT FONT_HEIGHT
#define CELL_COUNT                                                             \
  (OLED_SCREEN_HEIGHT / CELL_HEIGHT) * (OLED_SCREEN_WIDTH / CELL_WIDTH)
#define CELL_PADDING 5

// NSFS

// EEPROM pinout
#define EEPROM_SDA PC4
#define EEPROM_SCL PC5

#define EEPROM_SDA_PORT PORTC
#define EEPROM_SCL_PORT PORTC

#define EEPROM_SDA_DDR DDRC
#define EEPROM_SCL_DDR DDRC

#define EEPROM_A0 0
#define EEPROM_A1 0
#define EEPROM_A2 0

// NSP

// UART
#define UART_BAUD_RATE 9600

// GPIO
#define BTN_COUNT 3
#define BTN_PREV PC3
#define BTN_PREV_PORT PORTC
#define BTN_PREV_DDR DDRC
#define BTN_PREV_PIN PINC
#define BTN_NEXT PC1
#define BTN_NEXT_PORT PORTC
#define BTN_NEXT_DDR DDRC
#define BTN_NEXT_PIN PINC
#define BTN_APPLY PC2
#define BTN_APPLY_PORT PORTC
#define BTN_APPLY_DDR DDRC
#define BTN_APPLY_PIN PINC

void hardware_init(void);
uint8_t firmware_init(void);

#endif
