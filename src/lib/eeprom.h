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

#define EEPROM_SLA (0x50 | EEPROM_ADDRESS)
void eeprom_init(void);

void eeprom_write(uint16_t address, const uint8_t *data, uint16_t length);
void eeprom_read(uint16_t address, uint8_t *data, uint16_t length);

#endif
