#ifndef SLOT_H
#define SLOT_H

#include <stdint.h>

// Metadata
#define HEADER_VERSION 1 // 0-255
#define HEADER_METADATA_BYTES 8
// Header segment
// #define HEADER_ADDRESS 0x00
#define TABLE_ADDRESS 0x08
#define SLOT_MAX_FILENAME 8
#define SLOT_PER_SECTION 64
#define SLOT_SIZE 122
#define HEADER_SECTIONS 4
#define HEADER_MAX_SLOTS (SLOT_PER_SECTION * HEADER_SECTIONS)
// Data segment
#define DATA_ADDRESS                                                           \
	HEADER_METADATA_BYTES +                                                \
		HEADER_SECTIONS * SLOT_PER_SECTION * SLOT_MAX_FILENAME
#define SECTION_SIZE (SLOT_SIZE * SLOT_PER_SECTION)

typedef enum {
	SL_OK = 0,
	SL_WRONG_METADATA,
	SL_INVALID_FILENAME,
	SL_INVALID_SECTION,
	SL_INSUFFICIENT_SIZE,
	SL_NOT_FOUND,
	SL_SECTION_BUSY,
	SL_NOT_INITIALIZED,
	SL_NULL_ARGUMENTS,
	SL_NOT_ENOUGH_SPACE
} slot_ret_t;

typedef enum {
	SLOT_TXT_SECTION = 0b00,
	SLOT_EXE_SECTION = 0b01,
	SLOT_VEC_SECTION = 0b10,
	SLOT_IMG_SECTION = 0b11
} slot_section_t;

typedef enum {
	SECTION_TXT_ADDRESS = DATA_ADDRESS,
	SECTION_EXE_ADDRESS = DATA_ADDRESS + SECTION_SIZE,
	SECTION_VEC_ADDRESS = DATA_ADDRESS + 2 * SECTION_SIZE,
	SECTION_IMG_ADDRESS = DATA_ADDRESS + 3 * SECTION_SIZE,
} data_section_t;

typedef struct {
	uint8_t name[SLOT_MAX_FILENAME]; // non-null terminated string
} Slot;

// busy_slots[0] = text
// busy_slots[1] = exe
// busy_slots[2] = vec
// busy_slots[3] = img
extern uint64_t busy_slots[HEADER_SECTIONS];
extern uint8_t headers_initialized;

// software is now aware of header. never writes into hardware and only reads
// from it.
slot_ret_t slot_header_init(void);
// hardware level reset of header (by making all names null strings). never
// touches software and only reads from it.
void slot_table_format(void);

slot_ret_t slot_occupy(const Slot *new_slot, const slot_section_t section,
		       uint8_t *pos_in_section);
slot_ret_t slot_free(const Slot *slot, const slot_section_t section);
slot_ret_t slot_write(const slot_section_t section, const Slot *slot,
		      const uint8_t *data, const uint16_t data_size);
slot_ret_t slot_read(const slot_section_t section, const Slot *slot,
		     uint8_t *buf, const uint8_t buf_size);
#endif
