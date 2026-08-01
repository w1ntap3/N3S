#include "main.h"
#include "config.h"
#include "lib/eeprom.h"
#include "lib/oled.h"
#include "lib/ui.h"
#include <util/delay.h>

int main(void) {
  hardware_init();
  oled_init();
  oled_flush();
  return 0;
}
