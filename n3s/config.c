#include "config.h"
#include "lib/eeprom.h"
#include "lib/oled.h"
#include "lib/slot.h"
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>

cfg_ret_t hardware_init(void) {
  // clock_prescaler_off
  CLKPR = (1 << CLKPCE);
  CLKPR = 0;

  // OLED
  OLED_RES_DDR |= _BV(OLED_RES);
  OLED_DC_DDR |= _BV(OLED_DC);
  OLED_D0_DDR |= _BV(OLED_D0);
  OLED_D1_DDR |= _BV(OLED_D1);
  OLED_CS_DDR |= _BV(OLED_CS);

  // Buttons
  // Set as inputs and pull-up them
  BTN_PREV_DDR &= ~_BV(BTN_PREV);
  BTN_PREV_PORT |= _BV(BTN_PREV);
  BTN_APPLY_DDR &= ~_BV(BTN_APPLY);
  BTN_APPLY_PORT |= _BV(BTN_APPLY);
  BTN_NEXT_DDR &= ~_BV(BTN_NEXT);
  BTN_NEXT_PORT |= _BV(BTN_NEXT);

  oled_init();
  eeprom_init();
  return CFG_OK;
}

cfg_ret_t firmware_init(void) {
  slot_ret_t ret = slot_header_init();
  if (ret != SL_OK)
    return (cfg_ret_t)ret;
  return CFG_OK;
}
