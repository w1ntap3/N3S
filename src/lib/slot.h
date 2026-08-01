#ifndef SLOT_H
#define SLOT_H

#include <stdint.h>

// Metadata
#define HEADER_MAGIC0 0x41
#define HEADER_MAGIC1 0x5A
#define HEADER_MAGIC2 0x45
#define HEADER_VERSION 0 // 0-255
#define HEADER_BYTES 8
// Header table
#define HEADER_ADDRESS 0x08
#define SLOT_MAX_FILENAME 7
#define SLOT_TXT_TYPE 0b00
#define SLOT_EXE_TYPE 0b01
#define SLOT_VEC_TYPE 0b10
#define SLOT_IMG_TYPE 0b11
#define SLOT_PER_SECTION 64
#define SLOT_SIZE (SLOT_MAX_FILENAME + 1) // name bytes + type:slot byte = 8
#define HEADER_SECTIONS 4
#define HEADER_MAX_SLOTS (SLOT_PER_SECTION * HEADER_SECTIONS)

typedef struct {
  uint8_t type_and_sectionpos;  // 0:1 bits for type; 2:7 for slot number
                                // (relative to section)
  char name[SLOT_MAX_FILENAME]; // non-null terminated string
} Slot;

typedef enum { SL_OK = 0 } slot_ret_t;

// busy_slots[0] = text
// busy_slots[1] = exe
// busy_slots[2] = vec
// busy_slots[3] = img
extern uint64_t busy_slots[HEADER_SECTIONS];

slot_ret_t slot_header_init(void);
// make all names null strings
void slot_table_format(void);

// 0 - no free slots; returns 1-255 if assigned a slot
uint8_t slot_occupy(Slot new_slot);
slot_ret_t slot_free(uint8_t type, uint8_t slot);
slot_ret_t slot_write(uint8_t type, uint8_t slot, const uint8_t *data);
slot_ret_t slot_read(uint8_t type, uint8_t slot, uint8_t *buf);
// 0 - no slots found; returns 1-255 if found a slot
uint8_t slot_find(const uint8_t type, const char *name, uint8_t *slots_array);

#endif
