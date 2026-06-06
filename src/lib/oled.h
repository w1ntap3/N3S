#include "spi.h"

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

typedef enum
{
    OLED_OK = 0,
    OLED_ERROR
} oled_status;

oled_status oled_init();