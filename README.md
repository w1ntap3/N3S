# NSFS

# About the project

## Current stage: Implementation of the filesystem

- From-scratch UI API library (menu.c/.h) written from bare metal MMIO to API level graphics library. Supports rendering of pixels, lines, triangles, rectangles, characters/strings, menu items, loading bars, key:value pairs and render of files.
- A nano-scale filesystem on EEPROM with multiple file types supported with reading and writing abilities.
- PC<->MCU half-duplex communication through NSP (Nano Scale Protocol) over a USB-UART bridge with ability to write and retreat files to/from NSFS, dump the whole EEPROM to computer without losing/corrupting any of the data.

# Usage Notes

1. Text's Y position % 8 **must** be 0
2. Before starting the project call hardware_init() and oled_clear()
3. OLED screen has no ability to read data from the hardware. Call oled_flush() to write software graphical buffer into hardware.

## File types

### `.t` — Text

Multiple pages of ASCII text.

### `.b` — Bitmap image

A 128×64 bilevel bitmap image.

### `.v` — Vector image

A 128×64 vector-based image made of basic shapes with individual properties:

1. **Background** — normal or inverted
2. **Rectangle** — p1,p2 (`gfx_pixel`), fill
3. **Triangle** — p1,p2,p3 (`gfx_pixel`), fill
4. **Line** — p1,p2 (`gfx_pixel`)

### `.e` — Executable

When opened, the file can:

- Set/Toggle GPIO levels
- Encode PWM, for example to generate different buzzer notes

## PC interaction

### Writing files to NSFS

Use the web interface (or maybe CLI) to create NSFS files and transfer them to the MCU over NSP via USB-UART.

### Dumping EEPROM

You can smart-dump all NSFS files into a directory.
