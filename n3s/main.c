#include "main.h"
#include "config.h"
#include "lib/button.h"
#include "lib/ui.h"
#include <avr/io.h>
int main(void) {
  hardware_init();
  firmware_init();
  while (1) {
    poll_buttons();
    ui_draw_cell(20, 0, btn_states[BTN_PREV_INDEX] ? "1" : "0", 0);
    ui_draw_cell(60, 0, btn_states[BTN_APPLY_INDEX] ? "1" : "0", 0);
    ui_draw_cell(100, 0, btn_states[BTN_NEXT_INDEX] ? "1" : "0", 0);
  }
  return 0;
}
