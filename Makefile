#############################################################################
# 	tiny-oled.firmware - A project to push the limits of my abilities and
# 	understanding of embedded firmware development.
# 	Copyright (C) 2020 Stephen Murphy - github.com/stephendpmurphy
#
# 	Permission is hereby granted, free of charge, to any person obtaining a copy
# 	of this software and associated documentation files (the "Software"), to deal
# 	in the Software without restriction, including without limitation the rights
# 	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# 	copies of the Software, and to permit persons to whom the Software is
# 	furnished to do so, subject to the following conditions:
#
# 	The above copyright notice and this permission notice shall be included in all
# 	copies or substantial portions of the Software.
#
# 	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# 	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# 	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# 	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# 	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# 	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# 	SOFTWARE.
#############################################################################

# Target name
TARGET_APP=tiny-oled
TARGET_BOOT=boot

# MCU Type
MCU=atmega32u4

# Output directory
OUTPUT_DIR=./output

# Macro Definitions
F_CPU=8000000

# Toolchain
TOOLCHAIN_PREFIX=avr
CC=${TOOLCHAIN_PREFIX}-gcc
OBJCOPY=${TOOLCHAIN_PREFIX}-objcopy
OBJDUMP=${TOOLCHAIN_PREFIX}-objdump
SIZE=${TOOLCHAIN_PREFIX}-size

# Compiler Options
CFLAGS = -Wall -gstabs -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Os -mmcu=${MCU} -DF_CPU=${F_CPU}
CFLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections

# App source
SRC_APP =	$(wildcard ./src/*.c) \
			$(wildcard ./submodule/avr-ws2812/src/*.c) \
			$(wildcard ./submodule/u8g2/csrc/*.c) \
			$(wildcard ./submodule/bme280_driver/*.c) \

# Boot source
SRC_BOOT=./src/boot.c \

# Includes
INC=-I./inc \
	-I./submodule/avr-ws2812/inc \
	-I./submodule/u8g2/csrc \
	-I./submodule/bme280_driver \

all:
	mkdir -p ${OUTPUT_DIR}

	@echo "BUILDING APPLICATION"
	${CC} ${CFLAGS} ${INC} -o ${OUTPUT_DIR}/${TARGET_APP}.bin ${SRC_APP}
	${OBJCOPY} -j .text -j .data -O ihex ${OUTPUT_DIR}/${TARGET_APP}.bin ${OUTPUT_DIR}/${TARGET_APP}.hex
	${SIZE} -C --mcu=${MCU} ${OUTPUT_DIR}/${TARGET_APP}.bin

boot:
	mkdir -p ${OUTPUT_DIR}

	# Since we are building the bootloader.. Append the BUILD_BOOTLOADER macro to our definitions
	@echo "BUILDING BOOTLOADER"
	${CC} ${CFLAGS} -DBUILD_BOOTLOADER ${INC} -o ${OUTPUT_DIR}/${TARGET_BOOT}.bin ${SRC_BOOT}
	${OBJCOPY} -j .text -j .data -O ihex ${OUTPUT_DIR}/${TARGET_BOOT}.bin ${OUTPUT_DIR}/${TARGET_BOOT}.hex
	${SIZE} -C --mcu=${MCU} ${OUTPUT_DIR}/${TARGET_BOOT}.bin

test:
	@echo "RUNNING UNIT TESTS"
	cd ./tests/ ; ceedling test:all

flash:
	avrdude -p ${MCU} -B 1 -c avrispmkII -U flash:w:output/${TARGET_APP}.hex:i

flash_boot:
	avrdude -p ${MCU} -B 1 -c avrispmkII -U flash:w:output/${TARGET_APP}.hex:i

erase_chip:
	avrdude -p ${MCU} -c avrispmkII -e

read_fuses:
	avrdude -p ${MCU} -c avrispmkII -U lfuse:r:lfuse.txt:h
	avrdude -p ${MCU} -c avrispmkII -U hfuse:r:hfuse.txt:h
	avrdude -p ${MCU} -c avrispmkII -U efuse:r:efuse.txt:h

write_fuses:
	avrdude -p ${MCU} -c avrispmkII -U lfuse:w:0xde:m
	avrdude -p ${MCU} -c avrispmkII -U hfuse:w:0xd9:m
	avrdude -p ${MCU} -c avrispmkII -U efuse:w:0xcb:m

clean:
	rm -r -f ${OUTPUT_DIR}
	rm -r -f ./doc

docs:
	doxygen Doxyfile
	@echo "\n\nDOCUMENTS LOCATED AT: ${PWD}/doc/html/index.html\n\n"

