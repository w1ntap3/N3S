#ifndef TIMER_H
#define TIMER_H

#include <avr/interrupt.h>
#include <avr/io.h>
void timer0_init_ms(void);
ISR(TIMER0_COMPA_vect);
#endif
