// main.h:
// Main application code for NSFS

#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
extern uint8_t page;
extern uint8_t cursor;

typedef enum {
  POWER_ON = 0,
  INIT,
  MAIN_MENU,
  FILE_MENU,
  FILE_OPENED,
  FILE_PROPERTIES,
  FILE_NSP_SETTINGS,
  CONFIG_MENU,
  NSP_MODE,
} main_state_t;

typedef enum { MA_OK = 0, MA_INVALID_CURSOR } main_ret_t;
#endif
