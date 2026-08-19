#include "config.h"
int main(void) {
	hardware_init();
	firmware_init();
	while (1) {
		continue;
	}
	return 0;
}
