#include "spi.h"
#include <stdint.h>

// Register values
#define OLED_SPCR (SPI_SPCR_SPE | SPI_SPCR_DORD_MSB | SPI_SPCR_MSTR | SPI_SPCR_MODE0 | SPI_SPCR_DIV2)
#define OLED_SPSR (SPI_SPSR_SPI2X)

// Commands
#define OLED_CONTRAST_CONTROL 0x81
#define OLED_CONTRAST 255
#define OLED_SHOW_RAM 0xA4
#define OLED_IGNORE_RAM 0xA5
#define OLED_NORMAL 0xA6
#define OLED_INVERTED 0xA7
#define OLED_ON 0xAF
#define OLED_OFF 0xAE
#define OLED_CHARGE_PUMP_SETTING 0x8D
#define OLED_CHARGE_PUMP_ENABLE 0x14
#define OLED_START_ADDRESSING_MODE 0x20
#define OLED_HORIZONTAL_ADDRESSING 0b00
#define OLED_VERTICAL_ADDRESSING 0b01
#define OLED_PAGE_ADDRESSING 0b10
#define OLED_SET_PAGE_ADDRESS 0xB0
#define OLED_SET_LOWER_COLUMN 0x00
#define OLED_SET_HIGHER_COLUMN 0x10
#define OLED_RESET_PAGE_ADDRESS_HORIZONTAL 0x22
#define OLED_RESET_COLUMN_ADDRESS_HORIZONTAL 0x21

// Macro
#define Y_TO_PAGE(y) (y-(y%8))/8

typedef enum
{
    OLED_OK = 0,
    OLED_ERROR
} oled_status;

typedef struct {
    uint8_t x;
    uint8_t y;
} oled_pixel;

typedef struct {
    oled_pixel p1;
    oled_pixel p2;
} oled_line;

typedef struct {
    oled_pixel p1;
    oled_pixel p2;
    uint8_t fill;
} oled_rectangle;

typedef struct {
    oled_pixel p1;
    oled_pixel p2;
    oled_pixel p3;
    uint8_t fill;
} oled_triangle;

void oled_init(void);
void oled_clear(void);
oled_status oled_draw_pixel(oled_pixel pixel);
// Uses Bresenham's Line Algorithm
void oled_draw_line(oled_line line);
static void oled_draw_line_low_high(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t lowhigh);
void oled_draw_rectangle(oled_rectangle rectangle);
void oled_draw_triangle(oled_triangle triangle);