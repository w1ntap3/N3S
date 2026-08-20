#ifndef BUTTON_H
#define BUTTON_H

#include "../config.h"
#include <stdint.h>

#define DEBOUNCE_TIME_MS 15
#define SAMPLE_INTERVAL_MS 5
#define BTN_PREV_INDEX 0
#define BTN_PREV_SAMPLE                                                        \
  (uint8_t)(BTN_PREV_PIN & _BV(BTN_PREV)) // 0 - pressed; 1 - released
#define BTN_NEXT_INDEX 1
#define BTN_NEXT_SAMPLE                                                        \
  (uint8_t)(BTN_NEXT_PIN & _BV(BTN_NEXT)) // 0 - pressed; 1 - released
#define BTN_APPLY_INDEX 2
#define BTN_APPLY_SAMPLE                                                       \
  (uint8_t)(BTN_APPLY_PIN & _BV(BTN_APPLY)) // 0 - pressed; 1 - released
#define BTN_CONSISTENT_SAMPLES 3
typedef enum { MAIN_INIT = 0 } main_fsm_t;
typedef enum { BTN_PRESSED = 0, BTN_RELEASED } button_t;

extern button_t btn_states[BTN_COUNT];
extern uint8_t suspicion[BTN_COUNT];

void poll_buttons(void);

#endif
