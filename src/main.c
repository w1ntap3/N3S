#include "main.h"
#include "config.h"
#include "lib/gfx.h"
#include "lib/ui.h"
#include "util/delay.h"
#include <stdint.h>

const gfx_pixel aa = {.x = 0, .y = 0};

int main() {
  hardware_init();
  oled_clear();
  ui_draw_cell(aa.x, aa.y, "javid.t", 0);
  oled_flush();
  while (1) {
  }
  return 0;
}
