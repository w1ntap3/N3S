// 2) finish free
// 3) when occupying, fill rest of name with ' '
#include "slot.h"
#include "avr/sfr_defs.h"
#include "eeprom.h"
#include <stdint.h>
#include <string.h>

static const uint8_t metadata[HEADER_METADATA_BYTES] = {
	'A', 'Z', 'E', HEADER_VERSION, 0, 0, 0, 0,
};

static const uint8_t free_name[SLOT_MAX_FILENAME] = {0};

static const uint16_t section_address[HEADER_SECTIONS] = {
	DATA_ADDRESS,
	DATA_ADDRESS + SECTION_SIZE,
	DATA_ADDRESS + 2 * SECTION_SIZE,
	DATA_ADDRESS + 3 * SECTION_SIZE,
};

uint64_t busy_slots[HEADER_SECTIONS] = {0};
uint8_t headers_initialized = 0;

static slot_ret_t slot_find_by_name(const uint8_t *name,
				    const slot_section_t section,
				    uint8_t *slots_array,
				    const uint8_t slots_array_size,
				    uint8_t *found_count) {
	if (!name || !slots_array || !found_count)
		return SL_NULL_ARGUMENTS;
	if (section >= HEADER_SECTIONS)
		return SL_INVALID_SECTION;
	for (uint8_t c = 0; c < SLOT_MAX_FILENAME; c++) {
		if (name[c] == 0)
			return SL_INVALID_FILENAME;
	}

	uint8_t inspected_slot[SLOT_MAX_FILENAME] = {0};
	uint8_t current_element = 0;
	*found_count = 0;
	for (uint8_t slot_num = 0; slot_num < SLOT_PER_SECTION; slot_num++) {
		eeprom_read(TABLE_ADDRESS +
				    section * SLOT_PER_SECTION *
					    SLOT_MAX_FILENAME +
				    slot_num * SLOT_MAX_FILENAME,
			    inspected_slot, SLOT_MAX_FILENAME);
		if (inspected_slot[0] == 0)
			continue;
		if (!memcmp(inspected_slot, name, SLOT_MAX_FILENAME)) {
			if (current_element >= slots_array_size)
				return SL_INSUFFICIENT_SIZE;
			slots_array[current_element] = slot_num;
			current_element++;
			*found_count = current_element;
		}
	}
	if (current_element == 0)
		return SL_NOT_FOUND;
	return SL_OK;
}

static inline void update_slot_busy(uint16_t slot, uint8_t busy) {
	uint8_t section = slot / SLOT_PER_SECTION;
	uint8_t bit = slot % SLOT_PER_SECTION;

	if (busy)
		busy_slots[section] |= (1ULL << bit);
	else
		busy_slots[section] &= ~(1ULL << bit);
}

static inline slot_ret_t check_filename(const uint8_t *name) {
	for (uint8_t c = 0; c < SLOT_MAX_FILENAME; c++) {
		if (name[c] == 0)
			return SL_INVALID_FILENAME;
	}
	return SL_OK;
}

slot_ret_t slot_header_init(void) {
	headers_initialized = 0;
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
		uint8_t slot_row[SLOT_MAX_FILENAME];
		eeprom_read(TABLE_ADDRESS + current_slot * SLOT_MAX_FILENAME,
			    slot_row, SLOT_MAX_FILENAME);
		update_slot_busy(current_slot, slot_row[0] != 0);
	}
	headers_initialized = 1;
	return SL_OK;
}

void slot_table_format(void) {
	// Write Metadata Bytes
	eeprom_write(0, metadata, HEADER_METADATA_BYTES);

	// Reset the table
	uint8_t reset_page[EEPROM_PAGE_SIZE] = {0};
	for (uint16_t addr = TABLE_ADDRESS;
	     addr < TABLE_ADDRESS + HEADER_MAX_SLOTS * SLOT_MAX_FILENAME;
	     addr += EEPROM_PAGE_SIZE) {
		eeprom_write(addr, reset_page, EEPROM_PAGE_SIZE);
	}
}

slot_ret_t slot_occupy(const Slot *new_slot, const slot_section_t section,
		       uint8_t *pos_in_section) {
	// iterate through busy slots in the right section
	// find a free slot (0-63), fill it up with name on eeprom, return
	// position (as a parameter), update busy_slots
	if (!headers_initialized)
		return SL_NOT_INITIALIZED;
	if (!new_slot || !pos_in_section)
		return SL_NULL_ARGUMENTS;
	if (section >= HEADER_SECTIONS)
		return SL_INVALID_SECTION;
	if (check_filename(new_slot->name) == SL_INVALID_FILENAME)
		return SL_INVALID_FILENAME;
	for (uint8_t current_slot = 0; current_slot < SLOT_PER_SECTION;
	     current_slot++) {
		uint8_t is_busy = (busy_slots[section] >> current_slot) & 1;
		if (!is_busy) {
			eeprom_write(TABLE_ADDRESS +
					     SLOT_PER_SECTION * section *
						     SLOT_MAX_FILENAME +
					     current_slot * SLOT_MAX_FILENAME,
				     new_slot->name, SLOT_MAX_FILENAME);
			*pos_in_section = current_slot;
			update_slot_busy(
				section * SLOT_PER_SECTION + current_slot, 1);
			return SL_OK;
		}
	}
	return SL_SECTION_BUSY;
}
slot_ret_t slot_free(const Slot *slot, const slot_section_t section) {
	// Check for errors
	if (!headers_initialized)
		return SL_NOT_INITIALIZED;
	if (section >= HEADER_SECTIONS)
		return SL_INVALID_SECTION;
	if (!slot)
		return SL_NULL_ARGUMENTS;
	slot_ret_t err = check_filename(slot->name);
	if (err == SL_INVALID_FILENAME)
		return SL_INVALID_FILENAME;
	uint8_t slots_array[SLOT_PER_SECTION];
	uint8_t found_count;
	err = slot_find_by_name(slot->name, section, slots_array,
				SLOT_PER_SECTION, &found_count);
	if (err != SL_OK)
		return err;

	// Logic
	uint8_t slot_to_free = slots_array[found_count - 1];
	eeprom_write(TABLE_ADDRESS +
			     section * SLOT_PER_SECTION * SLOT_MAX_FILENAME +
			     slot_to_free * SLOT_MAX_FILENAME,
		     free_name, SLOT_MAX_FILENAME);
	update_slot_busy(section * SLOT_PER_SECTION + slot_to_free, 0);
	return SL_OK;
}

slot_ret_t slot_write(const slot_section_t section, const Slot *slot,
		      const uint8_t *data, const uint16_t data_size) {
	if (!headers_initialized)
		return SL_NOT_INITIALIZED;
	if (section >= HEADER_SECTIONS)
		return SL_INVALID_SECTION;
	if (!slot)
		return SL_NULL_ARGUMENTS;
	slot_ret_t err = check_filename(slot->name);
	if (err != SL_OK)
		return err;

	uint8_t slots_array[SLOT_PER_SECTION];
	uint8_t found_count;
	err = slot_find_by_name(slot->name, section, slots_array,
				SLOT_PER_SECTION, &found_count);
	if (err != SL_OK)
		return err;
	// if statement above checks for it but implicit trust sucks
	if (found_count == 0)
		return SL_NOT_FOUND;

	// Logic
	uint8_t write_slot = slots_array[found_count - 1];
	uint16_t slots_to_write = (uint16_t)data_size / (uint16_t)SLOT_SIZE;

	for (uint16_t i = 0; i < slots_to_write; i++) {
		uint16_t write_until = section_address[section] +
				       write_slot * SLOT_SIZE +
				       slots_to_write * SLOT_SIZE;
		if (write_until >
		    section_address[section] + SLOT_SIZE * SLOT_PER_SECTION)
			return SL_NOT_ENOUGH_SPACE;

		eeprom_write(section_address[section] + write_slot * SLOT_SIZE +
				     i * SLOT_SIZE,
			     data + i * SLOT_SIZE, SLOT_SIZE);
	}
	return SL_OK;
}
slot_ret_t slot_read(const slot_section_t section, const Slot *slot,
		     uint8_t *buf, const uint8_t buf_size);
