#ifndef EEPROM_H
#define EEPROM_H

#include "../config.h"
#include <stdint.h>

#define EEPROM_ADDRESS (EEPROM_A2 << 2) | (EEPROM_A1 << 1) | EEPROM_A0
#define EEPROM_PAGES 512
#define EEPROM_PAGE_SIZE 64 // bytes/words
#define EEPROM_WRITE_ADDRESS 0b10100000 | (EEPROM_ADDRESS << 1) | 0
#define EEPROM_READ_ADDRESS 0b10100000 | (EEPROM_ADDRESS << 1) | 1
#define EEPROM_TIME_WRITE 5 // ms

void eeprom_init(void);

void eeprom_byte_write(uint16_t address, uint8_t data);
void eeprom_page_write(uint16_t address, const uint8_t data[],
                       uint8_t data_count);
uint8_t eeprom_ready(void);

uint8_t eeprom_current_read();
uint8_t eeprom_random_read(uint16_t address);
void eeprom_sequential_read(uint16_t address, uint8_t *seq_data,
                            uint16_t data_count);

#endif
