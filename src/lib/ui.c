#include "ui.h"
#include "../config.h"
#include "font.h"
#include "gfx.h"

void ui_draw_cell(uint8_t x, uint8_t y, const char *text, uint8_t highlighted) {
  gfx_rectangle box = {.p1 = {.x = x, .y = y},
                       .p2 = {.x = x + CELL_WIDTH, .y = y + CELL_HEIGHT},
                       .fill = highlighted};
  gfx_pixel text_pos = {.x = x + 5, .y = y + 4};
  gfx_draw_rectangle(box);
  gfx_draw_string(text_pos, text, highlighted);
}

void ui_draw_spec(uint8_t y, const char *key, const char *value) {
  ui_draw_cell(0, y, key, 0);
  ui_draw_cell(OLED_SCREEN_WIDTH / 2, y, value, 0);
}

void ui_draw_progress_bar(const gfx_rectangle *rect, const char *caption,
                          uint8_t pct) {}
