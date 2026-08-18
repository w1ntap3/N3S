# N3S - Nano-Scale Sub-Systems
- N3S stands for Nano-Scale Sub-Systems. It combines a custom EEPROM Filesystem (NSFS), Display Library (NSD) and Communication Protocol (NSP), along with a CLI tool to interact with the system from Linux PC (NSCLI). (Fun fact: N3S sounds like entrées showing the nature of the project being made of entrée-esque subsystems).
- Project itself is meant for improving my skills in embedded/constrained environment C firmware development, systems engineering, writing portable code and modular software architecture.
- Every subsystem has each of these modular layers below designed and implemented manually (Inspired by the book Reusable Firmware Development by Jacob Beningo).
<p align="center">
  <img src="./reusable-firmware-architecture.png" alt="API Architecture from Beningo's book">
</p>
# NSFS - Nano-Scale Filesystem

- NSFS subsystem implements a custom filesystem on an EEPROM with 4 file types. It contains 4 modules that progress in abstraction. modules for slots and files are completely abstracted from hardware specific code and can be ran on any MCU with very few tweaks.

## Statistics
- Efficiently uses 96.9% of raw EEPROM bits meaning minimal waste.
- Lets users access and modify 89.06% of the raw EEPROM bits.
- Can store files up to 7.296KB in size.
- Allows up to 256 possible files

## Modules

| Module     | Responsibility |
| ---------- | -------------------------------------------------------------------------------------------- |
| i2c.h    | Low-level I2C driver for MCU–EEPROM communication                                            |
| eeprom.h | EEPROM read/write HAL built over I2C                                                         |
| slot.h   | Hardware-independent filesystem middleware: headers, metadata, slot tables, and data mapping |
| file.h   | File API; represents files as ordered slots within a section                                 |
## File types
- Text (.t) - Read ASCII text
- Executable (.e) - GPIO manipulation with custom timing
- Vector (.v) - Draw vector shapes
- Bitmap (.i) - Render bitmap images

# NSD - Nano-Scale Display

## Modules

| Module   | Responsibility |
| -------- | ----------------------------------------------------------------------------- |
| spi.h  | Low-level SPI driver for MCU–OLED communication                               |
| oled.h | OLED initialization, SPI commands, software framebuffer, hardware abstraction |
| gfx.h  | Coordinate-based graphics API for shapes and ASCII text                       |
| ui.h   | UI primitives: properties, buttons, loading screens, etc.                     |
| menu.h | Superloop-controlled menus combining UI with other subsystem APIs             |
## Shapes and Text
### Shapes
- Dots: Literal pixels
- Lines: Bresenham’s Line Algorithm implementation
- Rectangles: By diagonal line's endpoints
- Triangles By 3 vertices
### Text
- Rendering a 5x8 ASCII font defined in font.h
- Character composition into strings

# NSP - Nano-Scale Protocol

# NSCLI - Nano-Scale Command Line Interface
