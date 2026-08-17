#include "file.h"
#include "slot.h"
#include <stdint.h>
#include <string.h>

static inline file_ret_t validate_name(const char *name) { return FL_OK; }

static inline slot_section_t map_to_section(const char *name,
					    const uint8_t name_len) {
	char section = name[name_len - 1];
	switch (section) {
	case 't':
		return SLOT_TXT_SECTION;
		break;
	case 'e':
		return SLOT_EXE_SECTION;
		break;
	case 'v':
		return SLOT_VEC_SECTION;
		break;
	case 'i':
		return SLOT_IMG_SECTION;
		break;
	default:
		return SLOT_INVALID_SECTION;
		break;
	}
}

file_ret_t file_create(const char *name) {
	if (validate_name(name) != FL_OK)
		return FL_INVALID_NAME;

	uint8_t name_len = strlen(name);
	slot_section_t slot_section = map_to_section(name, name_len);
	// name_len-2 to remove .t/.e/.v/.i
	// NULL pos_in_section because we don't care where its assigned
	// First slot is always ordered 1
	slot_ret_t ret = slot_occupy(name, name_len - 2, slot_section, NULL, 1);
	if (ret == SL_OK)
		return FL_OK;
}
file_ret_t file_delete(const char *name);
file_ret_t file_append(const char *name, const uint8_t *data,
		       uint16_t data_size);
file_ret_t file_truncate(const char *name, const uint16_t size);
file_ret_t file_write(const char *name, const char *data,
		      const uint16_t *data_size);
file_ret_t file_read(const char *name, const uint16_t offset,
		     const uint8_t *buf, const uint16_t buf_size);
uint16_t file_size(const char *name);
