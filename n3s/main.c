#include "main.h"
#include "config.h"
#include "lib/button.h"
#include "lib/ui.h"
#include <stdalign.h>
#include <stdint.h>
#include <stdio.h>

static inline uint8_t power_on(void);
static inline main_ret_t main_menu();

uint8_t page = 0;
uint8_t cursor = 0;
main_state_t main_state = POWER_ON;

int main(void) {
  while (1) {
    switch (main_state) {
    case POWER_ON:
      if (power_on() != CFG_OK)
        return CFG_COULDNT_INITIALIZE;
      page = 0;
      cursor = 0;
      main_state = INIT;
    case INIT:
      main_menu();
      break;
    case MAIN_MENU:
      break;
    case FILE_MENU:
      break;
    case FILE_OPENED:
      break;
    case FILE_PROPERTIES:
      break;
    case FILE_NSP_SETTINGS:
      break;
    case CONFIG_MENU:
      break;
    case NSP_MODE:
      break;
    }
    poll_buttons();
  }
  return 0;
}

static inline uint8_t power_on() {
  cfg_ret_t ret;
  // TODO in future: There are no returns for hardware initializing but I
  // can add explicit checks so hardware fails can be detected
  ret = hardware_init();
  if (ret != CFG_OK)
    return CFG_COULDNT_INITIALIZE;
  ret = firmware_init();
  if (ret != CFG_OK) {
    while (1) {
      ui_draw_cell(10, 16, "Couldn't initialize", 1);
      ui_draw_cell(15, 32, "the firmware...", 1);
      char error_str[5] = {' ', ' ', ' ', ' ', ' '};
      snprintf(error_str, 5, "%d", ret);
      ui_draw_cell(20, 40, error_str, 1);
    }
    // dead code but whatever
    return CFG_COULDNT_INITIALIZE;
  }
  return CFG_OK;
}

static inline main_ret_t main_menu() {
  if (cursor >= CELL_COUNT)
    return MA_INVALID_CURSOR;

  ui_draw_cell(0, 0, "config", cursor == 0);
  ui_draw_cell(CELL_WIDTH, 0, "nsp mode", cursor == 1);
  file_list(file_names, &file_count);
  for (uint8_t file_cell = 0; file_cell < file_count; file_cell++) {
    ui_draw_cell(file_cell, 0, file_names[file_cell], (i + 2) == cursor);
  }
  return MA_OK;
}
