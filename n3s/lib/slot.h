#ifndef SLOT_H
#define SLOT_H

#include <stdint.h>

#define HEADER_VERSION 2 // 0-255
#define HEADER_METADATA_BYTES 8
#define SLOT_ENTRIES_ADDRESS 0x08
#define SLOT_MAX_FILENAME 8
#define SLOT_MIN_FILENAME 1
#define SLOTS_PER_SECTION 64
#define SLOT_ENTRY_SIZE 10
#define SLOT_DATA_BYTES 114
#define SECTION_SIZE (SLOT_DATA_BYTES * SLOTS_PER_SECTION)
#define HEADER_SECTIONS 4
#define HEADER_MAX_SLOTS (SLOTS_PER_SECTION * HEADER_SECTIONS)
#define DATA_ADDRESS                                                           \
  HEADER_METADATA_BYTES + HEADER_SECTIONS * SLOTS_PER_SECTION * SLOT_ENTRY_SIZE
#define SLOT_WRITE_OFFSET                                                      \
  (SLOT_MAX_FILENAME + 1) // SLOT_MAX_FILENAME amount of bytes for name +
                          // 1 order byte
typedef enum {
  SL_OK = 0,
  SL_WRONG_METADATA,
  SL_INVALID_SLOT,
  SL_INVALID_SECTION,
  SL_NO_WRITE_DATA,
  SL_WRITE_TOO_BIG,
  SL_BUFFER_TOO_SMALL,
  SL_INVALID_BUFFER,
  SL_NOT_INITIALIZED,
  SL_INVALID_NAME,
  SL_SECTION_BUSY,
  SL_ALREADY_FREE,
  SL_WRONG_NAME_SIZE,
  SL_SAME_ORDER
} slot_ret_t;

typedef enum {
  SLOT_TXT_SECTION = 0b00,
  SLOT_EXE_SECTION = 0b01,
  SLOT_VEC_SECTION = 0b10,
  SLOT_IMG_SECTION = 0b11,
  SLOT_INVALID_SECTION
} slot_section_t;

typedef struct {
  uint8_t name[SLOT_MAX_FILENAME];
  uint8_t order;
  uint8_t write_offset;
} Slot;

// busy_slots[0] = text
// busy_slots[1] = exe
// busy_slots[2] = vec
// busy_slots[3] = img
extern uint64_t busy_slots[HEADER_SECTIONS];
extern uint8_t slots_initialized;

// software is now aware of header. never writes into hardware and only reads
// from it.
slot_ret_t slot_header_init(void);
// hardware level reset of header (by making all names null strings). never
// touches software and only reads from it.
void slot_table_format(void);

slot_ret_t slot_occupy(const char *name, const uint8_t name_length,
                       const slot_section_t section, uint8_t *pos_in_section,
                       uint8_t order);
slot_ret_t slot_free(const slot_section_t section, const uint8_t entry);
slot_ret_t slot_update_metadata(const slot_section_t section,
                                const uint8_t entry, const uint8_t order,
                                const uint8_t write_offset);
slot_ret_t slot_write(const slot_section_t section, const uint8_t entry,
                      const uint8_t *data, const uint8_t data_size);
slot_ret_t slot_read(const slot_section_t section, const uint8_t entry,
                     uint8_t *buf, const uint8_t buf_size);
slot_ret_t slot_get_by_name(const uint8_t *name, const uint8_t name_len,
                            const slot_section_t section,
                            uint8_t *ordered_entries, uint8_t *entry_count);
#endif
