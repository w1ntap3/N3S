#include "file.h"
#include "slot.h"
#include <stdint.h>

static inline slot_section_t map_to_section(const uint8_t section) {
  switch (section) {
  case 't':
    return SLOT_TXT_SECTION;
  case 'e':
    return SLOT_EXE_SECTION;
  case 'v':
    return SLOT_VEC_SECTION;
  case 'i':
    return SLOT_IMG_SECTION;
  default:
    return SLOT_INVALID_SECTION;
  }
}

static inline file_ret_t validate_name(const char *name, const uint8_t name_len,
                                       uint8_t *section) {
  if (!name)
    return FL_NULL_NAME;
  if (name_len > FILE_MAX_FILENAME || name_len < FILE_MIN_FILENAME)
    return FL_WRONG_NAME_SIZE;

  uint8_t char_after_dot = 0;
  uint8_t dots = 0;
  for (uint8_t c = 0; c < name_len; c++) {
    if (name[c] == '.') {
      dots++;
      if (c != name_len - 2) {
        return FL_INVALID_NAME;
      } else {
        char_after_dot = name[c + 1];
      }
    }
  }
  if (dots == 0)
    return FL_INVALID_NAME;
  slot_section_t ret = map_to_section(char_after_dot);
  if (ret == SLOT_INVALID_SECTION) {
    return FL_INVALID_TYPE;
  }

  *section = ret;
  return FL_OK;
}

file_ret_t file_create(const char *name, const uint8_t name_len) {
  uint8_t section;
  file_ret_t file_ret = validate_name(name, name_len, &section);
  if (file_ret != FL_OK)
    return file_ret;
  uint8_t label_len = name_len - 2;
  uint8_t pos_in_section;
  slot_ret_t slot_ret =
      slot_occupy(name, label_len, section, &pos_in_section, 0);

  if (slot_ret == SL_OK)
    return FL_OK;
  if (slot_ret == SL_NOT_INITIALIZED)
    return FL_NOT_INITIALIZED;
  if (slot_ret == SL_SECTION_BUSY)
    return FL_TYPE_FULL;
  return FL_UNEXPECTED_ERROR;
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
