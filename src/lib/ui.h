// ui.h:
// Middleware for UI elements drawn using gfx.h

#ifndef UI_H
#define UI_H

#include "gfx.h"

void ui_draw_cell(uint8_t x, uint8_t y, const char *text, uint8_t highlighted);
void ui_draw_spec(uint8_t y, const char *key, const char *value);
void ui_draw_progress_bar(const gfx_rectangle *rect, const char *caption,
                          uint8_t pct);
#endif
