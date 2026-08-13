#include "ui.h"
#include "../config.h"
#include "font.h"
#include "gfx.h"

void ui_draw_cell(uint8_t x, uint8_t y, const char *text, uint8_t highlighted) {
  // gfx_rectangle box = {.p1 = {.x = x, .y = y},
  // .p2 = {.x = x + CELL_WIDTH, .y = y + CELL_HEIGHT},
  // .fill = highlighted};
  gfx_pixel text_pos = {.x = x + CELL_PADDING, .y = y + 0};
  // gfx_draw_rectangle(box);
  gfx_draw_string(text_pos, text, highlighted);
}

void ui_draw_spec(uint8_t y, const char *key, const char *value) {
  gfx_pixel pos = {.x = 0, .y = y};
  char key_value_pair[OLED_SCREEN_WIDTH / FONT_WIDTH + 1];
  char *p = key_value_pair;
  char *end = key_value_pair + OLED_SCREEN_WIDTH / FONT_WIDTH;
  while (*key && p < end)
    *p++ = *key++;
  if (p < end)
    *p++ = ':';
  while (*value && p < end)
    *p++ = *value++;
  *p = '\0';
  gfx_draw_string(pos, key_value_pair, 0);
}

void ui_draw_progress_bar(const char *caption, uint8_t pct) {
  oled_clear();
  gfx_rectangle bar = {.p1 = {.x = 10, .y = 40}, .p2 = {.x = 110, .y = 50}};
  gfx_rectangle loaded = {bar.p1, .p2 = {.x = 10 + pct, .y = 50}, .fill = 1};
  gfx_pixel text_pos = {.x = 10, .y = 24};
  gfx_draw_string(text_pos, caption, 0);
  gfx_draw_rectangle(bar);
  gfx_draw_rectangle(loaded);
}
