#include "eeprom.h"
#include "i2c.h"

static inline void eeprom_byte_write(uint16_t address, uint8_t data) {
  uint8_t buf[3];
  buf[0] = (uint8_t)(address >> 8);
  buf[1] = (uint8_t)(address & 0xFF);
  buf[2] = data;
  tw_master_transmit(EEPROM_SLA, buf, 3, false);
}

static inline void eeprom_page_write(uint16_t address, const uint8_t data[],
                                     uint8_t data_count) {
  if (!data || data_count == 0)
    return;
  uint8_t buf[2 + data_count];
  buf[0] = (uint8_t)(address >> 8);
  buf[1] = (uint8_t)(address & 0xFF);
  for (uint8_t i = 0; i < data_count; i++)
    buf[2 + i] = data[i];
  tw_master_transmit(EEPROM_SLA, buf, 2 + data_count, false);
}

static inline uint8_t eeprom_current_read(void) {
  uint8_t data;
  tw_master_receive(EEPROM_SLA, &data, 1);
  return data;
}

static inline uint8_t eeprom_random_read(uint16_t address) {
  uint8_t addr[2];
  uint8_t data;
  addr[0] = (uint8_t)(address >> 8);
  addr[1] = (uint8_t)(address & 0xFF);
  tw_master_transmit(EEPROM_SLA, addr, 2, true);
  tw_master_receive(EEPROM_SLA, &data, 1);
  return data;
}

static inline void eeprom_sequential_read(uint16_t address, uint8_t *seq_data,
                                          uint16_t data_count) {
  if (!seq_data || data_count == 0)
    return;

  uint8_t addr[2];
  addr[0] = (uint8_t)(address >> 8);
  addr[1] = (uint8_t)(address & 0xFF);
  tw_master_transmit(EEPROM_SLA, addr, 2, true);
  tw_master_receive(EEPROM_SLA, seq_data, (uint8_t)data_count);
}

static inline uint8_t eeprom_ready(void) {
  return (tw_master_transmit(EEPROM_SLA, 0, 0, false) == SUCCESS);
}

void eeprom_init(void) { tw_init(TW_FREQ_400K, false); }

void eeprom_write(uint16_t address, const uint8_t *data, uint16_t length) {
  if ((uint32_t)address + length > EEPROM_PAGES * EEPROM_PAGE_SIZE)
    return;
  while (length > 0) {
    uint16_t bytes_in_page = EEPROM_PAGE_SIZE - (address % EEPROM_PAGE_SIZE);
    uint16_t chunk = length < bytes_in_page ? length : bytes_in_page;
    eeprom_page_write(address, data, (uint8_t)chunk);
    while (!eeprom_ready())
      ;
    address += chunk;
    data += chunk;
    length -= chunk;
  }
}

void eeprom_read(uint16_t address, uint8_t *data, uint16_t length) {
  if ((uint32_t)address + length > EEPROM_PAGES * EEPROM_PAGE_SIZE)
    return;
  eeprom_sequential_read(address, data, length);
}
