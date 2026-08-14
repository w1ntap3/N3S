#include "slot.h"
#include "eeprom.h"
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

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

static inline slot_ret_t check_name(const char *name,
				    const uint8_t name_length) {
	if (!name || (name_length == 0) || name_length > SLOT_MAX_FILENAME)
		return SL_INVALID_NAME;
	for (uint8_t c = 0; c < name_length; c++) {
		if (name[c] == '\0')
			return SL_INVALID_NAME;
	}
	return SL_OK;
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

slot_ret_t slot_occupy(const char *name, const uint8_t name_length,
		       const slot_section_t section, uint8_t *pos_in_section,
		       uint8_t order) {
	if (!slots_initialized)
		return SL_NOT_INITIALIZED;
	slot_ret_t err = check_name(name, name_length);
	if (err != SL_OK)
		return err;
	if (section >= HEADER_SECTIONS)
		return SL_INVALID_SECTION;
	if (!pos_in_section)
		return SL_INVALID_BUFFER;

	for (uint8_t entry = 0; entry < SLOTS_PER_SECTION; entry++) {
		if (busy_slots[section] & (1ULL << entry))
			continue;

		Slot new_entry;
		memset(new_entry.name, ' ', SLOT_MAX_FILENAME);
		memcpy(new_entry.name, name, name_length);
		new_entry.order = order;
		new_entry.write_offset = 0;

		eeprom_write(SLOT_ENTRIES_ADDRESS +
				     (section * SLOTS_PER_SECTION + entry) *
					     SLOT_ENTRY_SIZE,
			     (uint8_t *)&new_entry, SLOT_ENTRY_SIZE);
		update_slot_busy(section * SLOTS_PER_SECTION + entry, 1);

		*pos_in_section = entry;
		return SL_OK;
	}
	return SL_SECTION_BUSY;
}

slot_ret_t slot_update_metadata(const slot_section_t section,
				const uint8_t entry, const uint8_t order,
				const uint8_t write_offset) {
	if (section >= HEADER_SECTIONS)
		return SL_INVALID_SECTION;
	if (entry >= SLOTS_PER_SECTION)
		return SL_INVALID_SLOT;
	if (!slots_initialized)
		return SL_WRONG_METADATA;
	if (!(busy_slots[section] & (1ULL << entry)))
		return SL_INVALID_SLOT;
	if (write_offset > SLOT_DATA_BYTES)
		return SL_INVALID_SLOT;

	uint8_t meta[2] = {order, write_offset};
	eeprom_write(SLOT_ENTRIES_ADDRESS +
			     (section * SLOTS_PER_SECTION + entry) *
				     SLOT_ENTRY_SIZE +
			     SLOT_MAX_FILENAME,
		     meta, 2);
	return SL_OK;
}

slot_ret_t slot_free(const slot_section_t section, const uint8_t entry) {
	if (section >= HEADER_SECTIONS)
		return SL_INVALID_SECTION;
	if (entry >= SLOTS_PER_SECTION)
		return SL_INVALID_SLOT;
	if (!slots_initialized)
		return SL_WRONG_METADATA;
	if (!(busy_slots[section] & (1ULL << entry)))
		return SL_INVALID_SLOT;

	uint8_t empty_slot[SLOT_ENTRY_SIZE] = {0};
	eeprom_write(SLOT_ENTRIES_ADDRESS +
			     (section * SLOTS_PER_SECTION + entry) *
				     SLOT_ENTRY_SIZE,
		     empty_slot, SLOT_ENTRY_SIZE);

	update_slot_busy(section * SLOTS_PER_SECTION + entry, 0);
	return SL_OK;
}
slot_ret_t slot_write(const slot_section_t section, const uint8_t entry,
		      const uint8_t *data, const uint8_t data_size) {
	if (data_size == 0 || !data) {
		return SL_NO_WRITE_DATA;
	}
	if (section >= HEADER_SECTIONS)
		return SL_INVALID_SECTION;
	if (entry >= SLOTS_PER_SECTION)
		return SL_INVALID_SLOT;
	if (!slots_initialized)
		return SL_WRONG_METADATA;
	if (!(busy_slots[section] & (1ULL << entry)))
		return SL_INVALID_SLOT;

	uint8_t write_offset = 0;
	eeprom_read(SLOT_ENTRIES_ADDRESS +
			    (section * SLOTS_PER_SECTION + entry) *
				    SLOT_ENTRY_SIZE +
			    SLOT_WRITE_OFFSET,
		    &write_offset, 1);

	if (write_offset > SLOT_DATA_BYTES)
		return SL_INVALID_SLOT;
	if (data_size > (SLOT_DATA_BYTES - write_offset))
		return SL_WRITE_TOO_BIG;

	eeprom_write(section_address[section] + entry * SLOT_DATA_BYTES +
			     write_offset,
		     data, data_size);

	uint8_t new_write_offset = write_offset + data_size;

	eeprom_write(SLOT_ENTRIES_ADDRESS +
			     (section * SLOTS_PER_SECTION + entry) *
				     SLOT_ENTRY_SIZE +
			     SLOT_WRITE_OFFSET,
		     &new_write_offset, 1);
	return SL_OK;
}
slot_ret_t slot_read(const slot_section_t section, const uint8_t entry,
		     uint8_t *buf, const uint8_t buf_size) {
	if (section >= HEADER_SECTIONS)
		return SL_INVALID_SECTION;
	if (entry >= SLOTS_PER_SECTION)
		return SL_INVALID_SLOT;
	if (!slots_initialized)
		return SL_WRONG_METADATA;
	if (!(busy_slots[section] & (1ULL << entry)))
		return SL_INVALID_SLOT;
	if (!buf)
		return SL_INVALID_BUFFER;

	uint8_t write_offset = 0;
	eeprom_read(SLOT_ENTRIES_ADDRESS +
			    (section * SLOTS_PER_SECTION + entry) *
				    SLOT_ENTRY_SIZE +
			    SLOT_WRITE_OFFSET,
		    &write_offset, 1);

	if (write_offset > SLOT_DATA_BYTES)
		return SL_INVALID_SLOT;
	if (buf_size < write_offset)
		return SL_BUFFER_TOO_SMALL;

	eeprom_read(section_address[section] + entry * SLOT_DATA_BYTES, buf,
		    write_offset);
	return SL_OK;
}
