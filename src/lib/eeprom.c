#include "eeprom.h"
#include "i2c.h"

void eeprom_init(void) {}

void eeprom_byte_write(uint16_t address, uint8_t data) {}
void eeprom_page_write(uint16_t address, const uint8_t data[],
                       uint8_t data_count) {}
uint8_t eeprom_ready(void) {}

uint8_t eeprom_current_read(void) {}
uint8_t eeprom_random_read(uint16_t address) {}
void eeprom_sequential_read(uint16_t address, uint8_t *seq_data,
                            uint16_t data_count) {}
