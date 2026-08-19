#include "config.h"
#include "lib/eeprom.h"
#include "lib/oled.h"
#include "lib/slot.h"
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
void hardware_init(void) {
	// clock_prescaler_off
	CLKPR = (1 << CLKPCE);
	CLKPR = 0;

	// OLED
	OLED_RES_DDR |= _BV(OLED_RES);
	OLED_DC_DDR |= _BV(OLED_DC);
	OLED_D0_DDR |= _BV(OLED_D0);
	OLED_D1_DDR |= _BV(OLED_D1);
	OLED_CS_DDR |= _BV(OLED_CS);

	oled_init();
	eeprom_init();
}

uint8_t firmware_init(void) {
	slot_ret_t ret = slot_header_init();
	if (ret != SL_OK)
		return 1;
	return 0;
}

void nsfs_format(void) { slot_table_format(); }
