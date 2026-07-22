#include "main.h"
#include "config.h"
#include "lib/ui.h"
#include "lib/eeprom.h"
#include <util/delay.h>

static uint8_t failures;

static void test_line(uint8_t row, const char *label, uint8_t pass) {
    gfx_pixel pos = {.x = 0, .y = row * 8};
    char line[22];
    uint8_t i = 0;
    if (pass) {
        line[i++] = 'O';
        line[i++] = 'K';
    } else {
        line[i++] = '!';
        line[i++] = '!';
        failures++;
    }
    line[i++] = ' ';
    while (*label && i < 21)
        line[i++] = *label++;
    line[i] = '\0';
    gfx_draw_string(pos, line, !pass);
    oled_flush();
    _delay_ms(300);
}

int main(void) {
    uint8_t buf[8];
    failures = 0;

    hardware_init();
    eeprom_init();
    oled_clear();

    gfx_draw_string((gfx_pixel){.x = 0, .y = 0}, "EEPROM DIAGNOSTIC", 0);
    oled_flush();
    _delay_ms(600);

    /* --- byte write + random read @ 0x0000 --- */
    eeprom_byte_write(0x0000, 0x55);
    while (!eeprom_ready())
        ;
    test_line(1, "W/R 0x0000:55", eeprom_random_read(0x0000) == 0x55);

    /* --- byte write + random read @ 0x7FFF (last address) --- */
    eeprom_byte_write(0x7FFF, 0xAA);
    while (!eeprom_ready())
        ;
    test_line(2, "W/R 0x7FFF:AA", eeprom_random_read(0x7FFF) == 0xAA);

    /* --- current address read: sets counter via read at addr-1 --- */
    eeprom_byte_write(0x0042, 0xC3);
    while (!eeprom_ready())
        ;
    eeprom_random_read(0x0041);
    test_line(3, "CUR RD 0x0042", eeprom_current_read() == 0xC3);

    /* --- sequential read --- */
    eeprom_byte_write(0x0200, 0xDE);
    while (!eeprom_ready())
        ;
    eeprom_byte_write(0x0201, 0xAD);
    while (!eeprom_ready())
        ;
    eeprom_byte_write(0x0202, 0xBE);
    while (!eeprom_ready())
        ;
    eeprom_byte_write(0x0203, 0xEF);
    while (!eeprom_ready())
        ;
    eeprom_sequential_read(0x0200, buf, 4);
    test_line(4, "SEQ DEADBEEF",
              buf[0] == 0xDE && buf[1] == 0xAD && buf[2] == 0xBE &&
                  buf[3] == 0xEF);

    /* --- page write + sequential read --- */
    {
        const uint8_t page[8] = {0x01, 0x23, 0x45, 0x67,
                                 0x89, 0xAB, 0xCD, 0xEF};
        eeprom_page_write(0x0300, page, 8);
        while (!eeprom_ready())
            ;
        eeprom_sequential_read(0x0300, buf, 8);
        uint8_t page_ok = 1;
        for (uint8_t j = 0; j < 8; j++)
            page_ok &= (buf[j] == page[j]);
        test_line(5, "PAGE 8B 0123..", page_ok);
    }

    /* --- final verdict on bottom row --- */
    test_line(7, "ALL PASS", failures == 0);

    while (1)
        ;
    return 0;
}
