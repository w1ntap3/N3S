#include "slot.h"
#include "eeprom.h"
#include <stdint.h>

static void update_busy_slots(uint8_t *slot_row) {
  uint8_t is_busy = (slot_row[1] != 0);
  uint8_t type = (slot_row[0] >> 6);
  uint8_t slot_number = slot_row[0] & 0b00111111;

  if (is_busy) {
    busy_slots[type] |= (1ULL << slot_number);
  } else {
    busy_slots[type] &= ~(1ULL << slot_number);
  }
}

slot_ret_t slot_header_init(void) {
  // Check Metadata Bytes
  uint8_t metadata[HEADER_BYTES];
  eeprom_read(0, metadata, HEADER_BYTES);
  // Set busy_slots
  for (int i = 0; i < HEADER_MAX_SLOTS; i++) {
    uint8_t slot_row[SLOT_SIZE];
    eeprom_read(HEADER_ADDRESS + i * SLOT_SIZE, slot_row, SLOT_SIZE);
    update_busy_slots(slot_row);
  }
  return SL_OK;
}

void slot_table_format(void) {
  // Write Metadata Bytes
  uint8_t metadata[HEADER_BYTES] = {0};
  metadata[0] = HEADER_MAGIC0;
  metadata[1] = HEADER_MAGIC1;
  metadata[2] = HEADER_MAGIC2;
  metadata[3] = HEADER_VERSION;
  metadata[4] = 0; // reserved
  metadata[5] = 0; // reserved
  metadata[6] = 0; // reserved
  metadata[7] = 0; // reserved
  eeprom_write(0, metadata,
               HEADER_BYTES); // fix overwriting top of the table

  // Reset the table
}

uint8_t slot_occupy(Slot new_slot);
slot_ret_t slot_free(uint8_t type, uint8_t slot);
slot_ret_t slot_write(uint8_t type, uint8_t slot, const uint8_t *data);
slot_ret_t slot_read(uint8_t type, uint8_t slot, uint8_t *buf);
uint8_t slot_find(const uint8_t type, const char *name, uint8_t *slots_array);
