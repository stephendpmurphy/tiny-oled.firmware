#############################################################################
#
#   tiny-oled.firmware - A project to the limits of my abilities and 
#   understanding of embedded firmware development.
#   Copyright (C) 2020 Stephen Murphy - github.com/stephendpmurphy
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
#############################################################################

# Target name
TARGET_APP=tiny-oled
TARGET_BOOT=boot

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

# App source
SRC_APP=./src/main.c \

# Boot source
SRC_BOOT=./src/boot.c \

# Includes
INC=-I./inc \

all:
	mkdir -p ${OUTPUT_DIR}

	echo "BUILDING APPLICATION"
	${CC} ${CFLAGS} ${INC} -o ${OUTPUT_DIR}/${TARGET_APP}.bin ${SRC_APP}
	${OBJCOPY} -j .text -j .data -O ihex ${OUTPUT_DIR}/${TARGET_APP}.bin ${OUTPUT_DIR}/${TARGET_APP}.hex

	echo "BUILDING BOOTLOADER"
	${CC} ${CFLAGS} ${INC} -o ${OUTPUT_DIR}/${TARGET_BOOT}.bin ${SRC_BOOT}
	${OBJCOPY} -j .text -j .data -O ihex ${OUTPUT_DIR}/${TARGET_BOOT}.bin ${OUTPUT_DIR}/${TARGET_BOOT}.hex

app:
	echo "BUILDING APPLICATION"
	mkdir -p ${OUTPUT_DIR}
	${CC} ${CFLAGS} ${INC} -o ${OUTPUT_DIR}/${TARGET_APP}.bin ${SRC_APP}
	${OBJCOPY} -j .text -j .data -O ihex ${OUTPUT_DIR}/${TARGET_APP}.bin ${OUTPUT_DIR}/${TARGET_APP}.hex

boot:
	echo "BUILDING BOOTLOADER"
	mkdir -p ${OUTPUT_DIR}
	${CC} ${CFLAGS} ${INC} -o ${OUTPUT_DIR}/${TARGET_BOOT}.bin ${SRC_BOOT}
	${OBJCOPY} -j .text -j .data -O ihex ${OUTPUT_DIR}/${TARGET_BOOT}.bin ${OUTPUT_DIR}/${TARGET_BOOT}.hex

flash_app:
	avrdude -p ${MCU} -c usbasp -U flash:w:${TARGET_APP}.hex:i -F -P usb

flash_boot:
	avrdude -p ${MCU} -c usbasp -U flash:w:${TARGET_BOOT}.hex:i -F -P usb

clean:
	rm -r -f ${OUTPUT_DIR}

