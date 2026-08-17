#ifndef FILE_H
#define FILE_H

#include "slot.h"
#include <stdint.h>

typedef enum { FL_OK = 0, FL_INVALID_NAME } file_ret_t;

file_ret_t file_create(const char *name);
file_ret_t file_delete(const char *name);
file_ret_t file_append(const char *name, const uint8_t *data,
		       uint16_t data_size);
file_ret_t file_truncate(const char *name, const uint16_t size);
file_ret_t file_write(const char *name, const char *data,
		      const uint16_t *data_size);
file_ret_t file_read(const char *name, const uint16_t offset,
		     const uint8_t *buf, const uint16_t buf_size);
uint16_t file_size(const char *name);

#endif // !FILE_H
