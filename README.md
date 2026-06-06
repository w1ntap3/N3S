# NSFS

# ‼️The README file is only a plan, features are still being implemented‼️

- A nano-scale filesystem on EEPROM with OLED display support.
- File types with custom metadata and on-screen rendering
- PC<->MCU half-duplex communication through NS-P (Nano Scale Protocol) over a USB-UART bridge

## File types

### `.t` — Text
Multiple pages of ASCII text.

### `.b` — Bitmap image
A 128×64 bilevel bitmap image.

### `.v` — Vector image
A 128×64 vector-based image made of basic shapes with individual properties:

1. **Background** — normal or inverted
2. **Rectangle** — `X`, `Y`, `Width`, `Length`, `Fill`, `Border`
3. **Triangle** — vertices `A`, `B`, `C`, `Fill`, `Border`
4. **Line** — `(X1, Y1) -> (X2, Y2)`

### `.e` — Executable
When opened, the file can:

- Set multiple GPIO pins to `HIGH` or `LOW` with defined delays
- Encode PWM, for example to generate buzzer frequencies
- Drive more complex behavior such as LED patterns with melody playback in the background

## PC interaction

### Writing files to NSFS
Use the web interface to create NSFS files and transfer them to the MCU over NS-P via USB-UART.

### Dumping EEPROM
You can smart-dump all NSFS files into a directory.