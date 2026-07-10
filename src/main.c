#include "main.h"
#include "config.h"
#include "lib/gfx.h"
#include "util/delay.h"
#include <stdint.h>

const gfx_pixel aa = {.x = 10, .y = 10};
const gfx_pixel ab = {.x = 50, .y = 40};
const gfx_pixel ac = {.x = 70, .y = 60};
const gfx_rectangle ad = {.p1 = aa, .p2 = ab, .fill = 1};
const gfx_rectangle ae = {.p1 = ab, .p2 = ac, .fill = 0};

int main() {
  hardware_init();
  oled_clear();
  oled_flush();
  gfx_draw_rectangle(ad);
  gfx_draw_rectangle(ae);
  oled_flush();
  while (1) {
  }
  return 0;
}
