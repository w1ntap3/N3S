#include "slot.h"
#include "eeprom.h"
#include <stdint.h>

static const uint8_t metadata[HEADER_METADATA_BYTES] = {
	'A', 'Z', 'E', HEADER_VERSION, 0, 0, 0, 0,
};

static const uint16_t section_address[HEADER_SECTIONS] = {
	DATA_ADDRESS,
	DATA_ADDRESS + SECTION_SIZE,
	DATA_ADDRESS + 2 * SECTION_SIZE,
	DATA_ADDRESS + 3 * SECTION_SIZE,
};

uint64_t busy_slots[HEADER_SECTIONS] = {0};
uint8_t slots_initialized = 0;

static inline void update_slot_busy(uint16_t slot, uint8_t busy) {
	uint8_t section = slot / SLOTS_PER_SECTION;
	uint8_t bit = slot % SLOTS_PER_SECTION;

	if (busy)
		busy_slots[section] |= (1ULL << bit);
	else
		busy_slots[section] &= ~(1ULL << bit);
}

slot_ret_t slot_header_init(void) {
	slots_initialized = 0;
	// Check Metadata Bytes
	uint8_t hardware_metadata[HEADER_METADATA_BYTES];
	eeprom_read(0, hardware_metadata, HEADER_METADATA_BYTES);
	for (uint8_t i = 0; i < HEADER_METADATA_BYTES; i++) {
		if (metadata[i] != hardware_metadata[i])
			return SL_WRONG_METADATA;
	}
	// Set busy_slots
	for (uint16_t current_slot = 0; current_slot < HEADER_MAX_SLOTS;
	     current_slot++) {
		Slot entry;
		eeprom_read(SLOT_ENTRIES_ADDRESS +
				    current_slot * SLOT_ENTRY_SIZE,
			    (uint8_t *)&entry, SLOT_ENTRY_SIZE);
		update_slot_busy(current_slot, entry.name[0] != 0);
	}
	slots_initialized = 1;
	return SL_OK;
}

void slot_table_format(void) {
	// Write Metadata Bytes
	eeprom_write(0, metadata, HEADER_METADATA_BYTES);

	// Reset the table
	uint8_t reset_page[EEPROM_PAGE_SIZE] = {0};
	for (uint16_t addr = SLOT_ENTRIES_ADDRESS;
	     addr < SLOT_ENTRIES_ADDRESS + HEADER_MAX_SLOTS * SLOT_ENTRY_SIZE;
	     addr += EEPROM_PAGE_SIZE) {
		eeprom_write(addr, reset_page, EEPROM_PAGE_SIZE);
	}

	// Reset busy slots so firmware is up to date with physical state
	for (uint8_t i = 0; i < HEADER_SECTIONS; i++)
		busy_slots[i] = 0;
	slots_initialized = 0;
}
