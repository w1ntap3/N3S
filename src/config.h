// SPI OLED pinout
#define OLED_D0 PB5 // D0 is SCLK in 4 wire SPI mode
#define OLED_D1 PB3 // D0 is SDIN in 4 wire SPI mode
#define OLED_RES PB1
#define OLED_DC PC5
#define OLED_CS PB2

#define OLED_D0_PORT PORTB
#define OLED_D1_PORT PORTB
#define OLED_RES_PORT PORTB
#define OLED_DC_PORT PORTC
#define OLED_CS_PORT PORTB

#define OLED_D0_DDR DDRB
#define OLED_D1_DDR DDRB
#define OLED_RES_DDR DDRB
#define OLED_DC_DDR DDRC
#define OLED_CS_DDR DDRB

// Screen resolution
#define OLED_SCREEN_WIDTH 128
#define OLED_SCREEN_HEIGHT 64
#define OLED_PAGES 8
#define OLED_SEG 128

// LED Debug pin
#define DEBUG_GPIO PD7