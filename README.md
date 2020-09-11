# tiny oled
![CI - AVR-GCC Build](https://github.com/stephendpmurphy/tiny-oled.firmware/workflows/CI%20-%20AVR-GCC%20Build/badge.svg) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
## Description
This project is intended to challenge myself and be used as a way to explore and try new concepts in a bare-metal 8bit environment. The current planned challenges are as follows
- [ ] Implement feature rich source in a memory constrained environment
- [X] Create a fully functioning development environment in Linux without using Atmel Studio 7 - This will involve using compilers, linkers, and debuggers via CL and/or utilizing VScode for a front end GUI while still leaving the underlying tools and technology open and configurable
- [X] Implement a 128x64 pixel OLED driver without using off the shelf libraries (Both from a challenge perspective, and a memory constraint that may come in to play with small amounts of Flash and SRAM)
- [ ] Integrate the FATFS lib for interfacing an SD card connected over SPI
- [ ] Add a 9 axis Gyro sensor over SPI
- [X] Add a temp and humidity sensor over SPI
- [ ] Create a bootloader capable of loading a new image to Flash via UART or load a file from an SD card
- [ ] Implement a WD timer for the sake of trying it.
- [ ] Add low power capabilities
- [X] Implement fancy addressable RGB LEDs
- [X] Implement a unit test framework (Unity & Ceedling)

## Hardware
For this project I will be using an [ATmega32u4 from Atmel/Microchip](https://www.microchip.com/wwwproducts/en/ATmega32u4). The part has a decent set of functionality to allow the device to be fully functional and usable without being overpower and overpriced.
- 32KB of Flash
- 2560B or SRAM
- 1024B of EEPROM
- 1x UART
- 2x SPI
- 1x I2c
- 1x Full Speed USB

The project for the hardware can be found in the [tiny-oled.hardware](https://github.com/stephendpmurphy/tiny-oled.hardware) repository on my profile. The project will be created and managed using the open source electrical design suite, [KiCad](https://kicad-pcb.org/).

# Getting started

#### Cloning & Retrieving source
Begin by cloning the repository:
```
git clone https://github.com/stephendpmurphy/tiny-oled.firmware.git
```

Checkout submodules:
```
git submodule --init --recursive
```

#### Installing Unity & CMock Unit Test Framework
This project uses Unity for Unit Testing and CMock to mock the hardware interfaces that we will be communicating with.</br>
First you will need to install Ruby so we can access *gem* to then retrieve the *Ceedling* tool.

```
$ sudo apt install ruby
$ sudo gem install ceedling
```

*TO-DO* - For info on the structure of tests in this project, review the documentation [here](*)

#### Installing AVR Tools & Utilities
The AVR toolchain is needed for compiling and *avrdude* for flashing AVR targets. To install the necessary tools:
```
$ sudo apt-get install gcc-avr avr-libc avrdude
```

# Compile, Test, Flash and AVR Fuses
#### Compiling
To compile the application:
```
$ make -j8
```

To compile the bootloader:
```
$ make boot
```

To clean the build *ouput/* directory:
```
$ make clean
```

#### Executing unit tests

To execute all unit tests. Move to the *tests* directory and executing the *ceedling* command:
```
$ cd tests/
$ ceedling
```

To execute a specific test. Execute the the *ceedling* command with the *test:$UNIT_TEST_NAME* option:
```
$ cd tests/
$ cd ceedling test:test_boot
```

Alternatively, you can execute all tests from the Makefile:
```
$ make test
```

#### Flashing
To erase the chip:
```
$ make erase_chip
```

To flash the application .hex:
```
$ make flash
```

To flash the bootloader .hex:
```
$ make flash_boot
```

#### Reading & Writing Fuses
Fuses are a set of configuration bytes in all AVR hardware that tell the chip things like what clock input and divider to use, enabling brown-out detection, enabling the Watchdog, and a few other features. The atmega32u4 has an internal 8Mhz clock that is then divided down by 8, to give a CPU clock of 1Mhz out of the box. We want to run the part faster however, so we need to disable the Clock divider bit in the lower fuse (lfuse) to give us a CPU clock of 8Mhz. You can calculate the correct value of the fuses using the [AVR Fuse Calc](https://www.engbedded.com/fusecalc/) or simply reference the Fuse section of the datasheet and calculate the correct value yourself.

To simplify things. You can execute the **write_fuses** option with **make** to write the appropriate fuse values for this project.
```
$ make write_fuses
```

And you can execute the **read_fuses** option with **make** to read out the lfuse, hfuse and efuse values into their respective .txt files
```
$ make read_fuses
```
