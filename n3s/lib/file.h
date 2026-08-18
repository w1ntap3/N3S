#ifndef FILE_H
#define FILE_H

#include "slot.h"
#include <stdint.h>

#define FILE_MAX_FILENAME 10
#define FILE_MIN_FILENAME 3

typedef enum {
  FL_OK = 0,
  FL_NAME_TOO_LONG,
  FL_INVALID_NAME,
  FL_INVALID_TYPE,
  FL_WRONG_NAME_SIZE,
  FL_NULL_NAME,
  FL_NOT_INITIALIZED,
  FL_TYPE_FULL,
  FL_UNEXPECTED_ERROR, // Sometimes used vaguely on purpose to signify some
                       // unpredictable behavior
  FL_INVALID_BUFFER,
  FL_SAME_ORDER_SLOTS
} file_ret_t;

file_ret_t file_create(const char *name, const uint8_t name_len);
file_ret_t file_delete(const char *name, const uint8_t name_len);
file_ret_t file_append(const char *name, const uint8_t *data,
                       uint16_t data_size);
file_ret_t file_truncate(const char *name, const uint16_t size);
file_ret_t file_write(const char *name, const char *data,
                      const uint16_t *data_size);
file_ret_t file_read(const char *name, const uint16_t offset,
                     const uint8_t *buf, const uint16_t buf_size);
uint16_t file_size(const char *name);

#endif // !FILE_H
