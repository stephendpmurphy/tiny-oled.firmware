# Target name
TARGET=tiny-oled

# MCU Type
MCU=attiny87

# Output directory
OUTPUT_DIR=./output

# Macro Definitions
F_CPU=1200000

# Toolchain
TOOLCHAIN_PREFIX=avr
CC=${TOOLCHAIN_PREFIX}-gcc
OBJCOPY=${TOOLCHAIN_PREFIX}-objcopy
OBJDUMP=${TOOLCHAIN_PREFIX}-objdump

# Compiler Options
CFLAGs=-std=C99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.

# Source
SRC=./src/main.c \

# Includes
INC=-I./inc \

all:
	rm -r -f ${OUTPUT_DIR}
	mkdir ${OUTPUT_DIR}
	${CC} ${CFLAGS} ${INC} -o ${OUTPUT_DIR}/${TARGET}.bin ${SRC}
	${OBJCOPY} -j .text -j .data -O ihex ${OUTPUT_DIR}/${TARGET}.bin ${OUTPUT_DIR}/${TARGET}.hex

flash:
	avrdude -p ${MCU} -c usbasp -U flash:w:${TARGET}.hex:i -F -P usb

clean:
	rm -r -f ${OUTPUT_DIR}

