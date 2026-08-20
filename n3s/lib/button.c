#include "button.h"

volatile button_t btn_states[BTN_COUNT] = {BTN_RELEASED, BTN_RELEASED,
                                           BTN_RELEASED};
uint8_t suspicion[BTN_COUNT] = {0, 0, 0};

static inline void button_event(const uint8_t btn, const uint8_t sample) {
  // there are no holding actions
  // we implicitly assume that if the current state happened because of
  // edge-change. by the time this function is called it means we just learned
  // about a transition in states. very simple logic
  switch (sample) {
  case BTN_RELEASED:
    break;
  case BTN_PRESSED:
    break;
  }
}
static inline void poll_single_button(const uint8_t btn, const uint8_t sample) {
  if (sample == btn_states[btn]) {
    suspicion[btn] = 0;
    return;
  }
  suspicion[btn]++;
  // Suspicion sustains long enough that its time to change our mind
  if (suspicion[btn] >= BTN_CONSISTENT_SAMPLES) {
    suspicion[btn] = 0;
    btn_states[btn] = sample;
    button_event(btn, sample);
  }
}

void poll_buttons(void) {
  for (uint8_t btn = 0; btn < BTN_COUNT; btn++) {
    switch (btn) {
    case BTN_PREV_INDEX:
      poll_single_button(BTN_PREV_INDEX, BTN_PREV_SAMPLE);
      break;
    case BTN_NEXT_INDEX:
      poll_single_button(BTN_NEXT_INDEX, BTN_NEXT_SAMPLE);
      break;
    case BTN_APPLY_INDEX:
      poll_single_button(BTN_APPLY_INDEX, BTN_APPLY_SAMPLE);
      break;
    }
  }
}
