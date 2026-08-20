#include "timer.h"
#include "button.h"
#include <avr/io.h>
#include <avr/sfr_defs.h>

void timer0_init_ms(void) {
  TCCR0A = (1 << WGM01);
  TCCR0B = (1 << CS01) | (1 << CS00);
  OCR0A = 124;
  TIMSK0 = _BV(OCIE0A);
}

ISR(TIMER0_COMPA_vect) { poll_buttons(); }
