#include "main.h"
#include "config.h"
#include "lib/gfx.h"
#include "lib/oled.h"
#include "lib/ui.h"
#include "util/delay.h"
#include <stdint.h>

int main() {
  hardware_init();

  while (1) {
    // Screen 1: File list (2 columns)
    oled_clear();
    ui_draw_cell(0, 0, "settings", 1);
    ui_draw_cell(64, 0, "data.bin", 0);
    ui_draw_cell(0, 8, "config.e", 0);
    ui_draw_cell(64, 8, "log.txt", 0);
    ui_draw_cell(0, 16, "cache.i", 0);
    ui_draw_cell(64, 16, "temp.v", 0);
    ui_draw_cell(0, 24, "backup.t", 0);
    ui_draw_cell(64, 24, "user.d", 0);
    ui_draw_cell(0, 32, "sys.e", 0);
    ui_draw_cell(64, 32, "app.i", 0);
    ui_draw_cell(0, 40, "net.v", 0);
    ui_draw_cell(64, 40, "usb.t", 0);
    ui_draw_cell(0, 48, "mem.e", 0);
    ui_draw_cell(64, 48, "cpu.i", 0);
    ui_draw_cell(0, 56, "pwr.v", 0);
    ui_draw_cell(64, 56, "io.t", 0);
    oled_flush();
    _delay_ms(2000);

    // Screen 2: Specs (2 columns)
    oled_clear();
    ui_draw_spec(0, "name", "sys");
    ui_draw_spec(8, "type", "app");
    ui_draw_spec(16, "size", "2k");
    ui_draw_spec(24, "date", "24");
    ui_draw_spec(32, "stat", "ok");
    ui_draw_spec(40, "mode", "rw");
    ui_draw_spec(48, "freq", "8M");
    ui_draw_spec(56, "ram", "2k");
    oled_flush();
    _delay_ms(2000);

    // Screen 3: Loading
    for (uint8_t i = 0; i <= 100; i += 10) {
      ui_draw_progress_bar("loading", i);
      oled_flush();
      _delay_ms(300);
    }

    // Screen 4: Graphics demo
    oled_clear();

    // Draw lines
    gfx_line l1 = {.p1 = {.x = 0, .y = 0}, .p2 = {.x = 127, .y = 31}};
    gfx_line l2 = {.p1 = {.x = 127, .y = 0}, .p2 = {.x = 0, .y = 31}};
    gfx_draw_line(l1);
    gfx_draw_line(l2);

    // Draw rectangles
    gfx_rectangle r1 = {
        .p1 = {.x = 10, .y = 40}, .p2 = {.x = 50, .y = 62}, .fill = 0};
    gfx_rectangle r2 = {
        .p1 = {.x = 70, .y = 40}, .p2 = {.x = 110, .y = 62}, .fill = 1};
    gfx_draw_rectangle(r1);
    gfx_draw_rectangle(r2);

    // Draw text
    gfx_pixel txt = {.x = 0, .y = 33};
    gfx_draw_string(txt, "gfx demo", 0);

    oled_flush();
    _delay_ms(2000);
  }

  return 0;
}
