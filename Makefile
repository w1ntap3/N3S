# AVR MAKEFILE FOR NSFS

# Hardware specifics
MCU := m328p
MMCU := atmega328p
PROGRAMMER := usbasp
XTAL := 125kHz
BAUD := 9600
F_CPU := 8000000UL

# Program specifics
TARGET := src/main
SRC := $(wildcard src/*.c) $(wildcard src/lib/*.c)

# Extra flags
FLAGS := -Os -Wall -flto -ffunction-sections -fdata-sections -Wl,-gc-sections -mrelax -DUART_RX0_BUFFER_SIZE=64 -DUART_TX0_BUFFER_SIZE=64 -std=gnu99 # Optimization flags
SERIAL_PORT := /dev/ttyUSB0

flash: compile
	avr-size --mcu=${MMCU} -C ${TARGET}
	sudo avrdude -p ${MCU} -c ${PROGRAMMER} -U flash:w:${TARGET}.hex:i -B ${XTAL}

debug: compile
	sudo avrdude -p ${MCU} -c ${PROGRAMMER} -U flash:w:${TARGET}.hex:i -B ${XTAL}
	sudo picocom -b ${BAUD} ${SERIAL_PORT}

compile: 
	avr-gcc -mmcu=${MMCU} -DF_CPU=${F_CPU} -o ${TARGET} ${FLAGS} ${SRC}
	avr-objcopy -O ihex -R .eeprom ${TARGET} ${TARGET}.hex

clean:
	rm -f ${TARGET} ${TARGET}.hex
