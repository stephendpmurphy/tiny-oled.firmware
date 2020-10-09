# Set min req version of Cmake
cmake_minimum_required(VERSION 3.10)

# Set project name
project("tiny oled firmware")

# Set product name
set(PRODUCT_NAME tiny-oled)

# Target MCU
set(MCU atmega32u4)

# Progammer type
set(PROG_TYPE avrispmkII)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

# Setup Toolchain Compiler and Linker
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER /usr/bin/avr-g++)
set(CMAKE_C_COMPILER /usr/bin/avr-gcc)
set(CMAKE_ASM_COMPILER /usr/bin/avr-gcc)
set(CMAKE_EXE_LINKER_FLAGS "-mmcu=${MCU} -Wl,--gc-sections")

# Project definitions for the CPU and USB clock speed
set(F_CPU 8000000UL)
set(F_USB 8000000UL)

# Add our definitions to CMake
add_definitions(
    -DF_CPU=${F_CPU}
    -DF_USB=${F_USB}
)

# Add our MCU compiler options
add_compile_options(
    -mmcu=${MCU} # MCU
    -std=gnu99 # C99 standard
    -Os # optimize
    -Wall # enable warnings
    -Wno-main
    -Wundef
    -Wstrict-prototypes
    -Werror
    -Wfatal-errors
    -Wl,--relax,--gc-sections
    -g
    -gdwarf-2
    -funsigned-char # a few optimizations
    -funsigned-bitfields
    -fpack-struct
    -fshort-enums
    -ffunction-sections
    -fdata-sections
    -fno-split-wide-types
    -fno-tree-scev-cprop
)

# Add all of our include directories to our INCLUDE var
set(INCLUDES ${CMAKE_SOURCE_DIR}/inc
             ${CMAKE_SOURCE_DIR}/inc/usb
             ${CMAKE_SOURCE_DIR}/inc/sd
             ${CMAKE_SOURCE_DIR}/submodule/avr-ws2812/inc
             ${CMAKE_SOURCE_DIR}/submodule/u8g2/csrc
             ${CMAKE_SOURCE_DIR}/submodule/bme280_driver
             ${CMAKE_SOURCE_DIR}/submodule/icm20948/inc
             ${CMAKE_SOURCE_DIR}/submodule/petit_fatfs/source
             ${CMAKE_SOURCE_DIR}/submodule/lufa/
             ${CMAKE_SOURCE_DIR}/submodule/lufa/LUFA
             ${CMAKE_SOURCE_DIR}/submodule/lufa/LUFA/Drivers/USB
             ${CMAKE_SOURCE_DIR}/submodule/lufa/LUFA/Drivers/USB/Core
             ${CMAKE_SOURCE_DIR}/submodule/lufa/LUFA/Drivers/USB/Core/AVR8
             ${CMAKE_SOURCE_DIR}/submodule/lufa/LUFA/Drivers/USB/Class
             ${CMAKE_SOURCE_DIR}/submodule/lufa/LUFA/Drivers/USB/Class/Common
             ${CMAKE_SOURCE_DIR}/submodule/lufa/LUFA/Drivers/USB/Class/Device
             ${CMAKE_SOURCE_DIR}/submodule/lufa/LUFA/Platform
)

# Include the directories into CMake
include_directories(${INCLUDES})

# Add our source files from our application
set(APP_SRC ${CMAKE_SOURCE_DIR}/src/display.c
                ${CMAKE_SOURCE_DIR}/src/climate.c
                ${CMAKE_SOURCE_DIR}/src/main.c
                ${CMAKE_SOURCE_DIR}/src/spi.c
                ${CMAKE_SOURCE_DIR}/src/telemetry.c
                ${CMAKE_SOURCE_DIR}/src/tick.c
                ${CMAKE_SOURCE_DIR}/src/usb/usb.c
                ${CMAKE_SOURCE_DIR}/src/usb/descriptors.c
                ${CMAKE_SOURCE_DIR}/src/sd/diskio.c
                ${CMAKE_SOURCE_DIR}/submodule/petit_fatfs/source/pff.c
)

# Add our source files from all of our submodules
FILE(GLOB AVR_WS2812_SRC "./submodule/avr-ws2812/src/*.c")
FILE(GLOB BME280_DRIVER_SRC "./submodule/bme280_driver/*.c")
FILE(GLOB ICM20948_SRC "./submodule/icm20948/src/*.c")
FILE(GLOB LUFA_SRC  "./submodule/lufa/LUFA/Platform/UC3/*.c"
                    "./submodule/lufa/LUFA/Drivers/USB/Class/Device/CDCClassDevice.c"
                    "./submodule/lufa/LUFA/Drivers/USB/Core/*.c"
                    "./submodule/lufa/LUFA/Drivers/USB/Core/AVR8/*.c")
FILE(GLOB U8G2_SRC "./submodule/u8g2/csrc/*.c")

# Create our executable
add_executable(${PRODUCT_NAME}  ${APP_SRC}
                                ${AVR_WS2812_SRC}
                                ${BME280_DRIVER_SRC}
                                ${ICM20948_SRC}
                                ${LUFA_SRC}
                                ${U8G2_SRC}
)

# Rename the output to .elf as we will create multiple files
set_target_properties(${PRODUCT_NAME} PROPERTIES OUTPUT_NAME ${PRODUCT_NAME}.elf)

# Strip binary for upload
add_custom_target(strip ALL avr-strip ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PRODUCT_NAME}.elf DEPENDS ${PRODUCT_NAME})

# Transform binary into hex file
add_custom_target(hex ALL avr-objcopy -j .text -j .data -O ihex ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PRODUCT_NAME}.elf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PRODUCT_NAME}.hex DEPENDS strip)

# Print out the binary size
add_custom_target(size ALL avr-size -C --mcu=${MCU} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PRODUCT_NAME}.elf DEPENDS hex)

# Upload the firmware with avrdude
add_custom_target(flash avrdude -c ${PROG_TYPE} -B 1 -p ${MCU} -U flash:w:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PRODUCT_NAME}.hex:i DEPENDS size)

# Erase the chip
add_custom_target(erase avrdude -p ${MCU} -c ${PROG_TYPE} -e)

# Write AVR Fuses
add_custom_target(write_fuses avrdude -p ${MCU} -c avrispmkII -U lfuse:w:0xde:m -U hfuse:w:0xd9:m -U efuse:w:0xcb:m)

# Read AVR Fuses
add_custom_target(read_fuses avrdude -p ${MCU} -c avrispmkII -U lfuse:r:${CMAKE_SOURCE_DIR}/lfuse.txt:h -U hfuse:r:${CMAKE_SOURCE_DIR}/hfuse.txt:h -U efuse:r:${CMAKE_SOURCE_DIR}/efuse.txt:h)

# Create Doxygen Docs
add_custom_target(docs doxygen ${CMAKE_SOURCE_DIR}/Doxyfile)
