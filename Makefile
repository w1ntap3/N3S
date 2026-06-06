# AVR MAKEFILE FOR NSFS

# Hardware specifics
MCU := m328p
MMCU := atmega328p
PROGRAMMER := usbasp
XTAL := 125kHz
BAUD := 115200
F_CPU := 8000000UL

# Program specifics
TARGET := src/main
SRC := $(wildcard src/*.c) $(wildcard src/lib/*.c)

# Extra flags
FLAGS := -Os -Wall # Small optimizations + all warnings
SERIAL_PORT := /sys/class/tty/ i need that usb-uart man

flash: compile
	sudo avrdude -p ${MCU} -c ${PROGRAMMER} -U flash:w:${TARGET}.hex:i -B ${XTAL}

debug: compile
	sudo avrdude -p ${MCU} -c ${PROGRAMMER} -U flash:w:${TARGET}.hex:i -B ${XTAL}

compile: 
	avr-gcc -mmcu=${MMCU} -DF_CPU=${F_CPU} -o ${TARGET} ${FLAGS} ${SRC}
	avr-objcopy -O ihex -R .eeprom ${TARGET} ${TARGET}.hex

clean:
	sudo rm ${TARGET}
	sudo rm ${TARGET}.hex