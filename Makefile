# AVR MAKEFILE FOR NSFS

# Hardware specifics
MCU := m328p
MMCU := atmega328p
PROGRAMMER := usbasp
XTAL := 125kHz
BAUD := 9600
F_CPU := 8000000UL

# Program specifics
TARGET := nsfs/main
SRC := $(wildcard nsfs/*.c) $(wildcard nsfs/lib/*.c)

# Extra flags
FLAGS := -Os -Wall -flto -ffunction-sections -fdata-sections -Wl,-gc-sections -mrelax
SERIAL_PORT := /dev/ttyUSB0

# CLI
BINDIR := /home/wintape/.local/bin/

flash: compile
	avr-size --mcu=${MMCU} -C ${TARGET}
	sudo avrdude -p ${MCU} -c ${PROGRAMMER} -U flash:w:${TARGET}.hex:i -B ${XTAL}

debug: compile
	sudo avrdude -p ${MCU} -c ${PROGRAMMER} -U flash:w:${TARGET}.hex:i -B ${XTAL}
	sudo picocom -b ${BAUD} ${SERIAL_PORT}

compile: 
	avr-gcc -mmcu=${MMCU} -DF_CPU=${F_CPU} -o ${TARGET} ${FLAGS} ${SRC}
	avr-objcopy -O ihex -R .eeprom ${TARGET} ${TARGET}.hex

cli:
	gcc -Wall -o nsfs-cli/nsfs nsfs-cli/main.c
	cp nsfs-cli/nsfs ${BINDIR}

clean:
	rm -f ${TARGET} ${TARGET}.hex nsfs-cli/nsfs
