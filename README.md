# ATtiny OLED

## Description
This project is intended to challenge myself and be used as a way to explore and try new concepts in a bare-metal 8bit environment. The current planned challenges are as follows
- Implement feature rich source in a memory constrained environment (8KB Flash & 512B SRAM)
- Create a fully functioning development environment in Linux without using Atmel Studio 7 - This will involve using compilers, linkers, and debuggers via CL and/or utilizing VScode for a front end GUI while still leaving the underlying tools and technology open and configurable
- Implement a 128x64 pixel OLED driver without using off the shelf libraries (Both from a challenge perspective, and a memory constraint that may come in to play with small amounts of Flash and SRAM)
- Integrate the FATFS lib for interfacing an SD card connected over SPI
- Create a bootloader capable of loading a new image to Flash via UART or load a file from an SD card
- Implement a WD timer for the sake of trying it. (lol)
- Add low power capabilities
- Implement fancy addressable RGB LEDs over I2C

## Hardware
For this project I will be using an [ATtiny87 from Atmel/Microchip](https://www.microchip.com/wwwproducts/en/ATTINY87). The part has a limited feature set and a small amount of memory, but hopefully enough to implement everything desired and provide an adequate challenge. 
- 8KB of Flash
- 512B or SRAM
- 512B of EEPROM
- 1x UART
- 2x SPI
- 1x I2c

The project for the hardware can be found in the [tiny-oled.hardware](https://github.com/stephendpmurphy/tiny-oled.hardware) repository on my profile. The project will be created and managed using the open source electrical design suite, [KiCad](https://kicad-pcb.org/).

## Tools
The entire project and installation is executed and used in Linux.

First start by installing *avr-gcc*, *avr-libs*, and *avrdude*
```
$ sudo apt-get install gcc-avr avr-libc avrdude
```

## Compiling
To compile run the following command
```
$ make
```
The output .bin and .hex files will be placed into the *output/* folder.

## Flashing
*TO-DO*

