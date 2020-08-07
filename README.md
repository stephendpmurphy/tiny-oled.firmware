# attiny-oled
![CI - AVR-GCC Build](https://github.com/stephendpmurphy/tiny-oled.firmware/workflows/CI%20-%20AVR-GCC%20Build/badge.svg)
## Description
This project is intended to challenge myself and be used as a way to explore and try new concepts in a bare-metal 8bit environment. The current planned challenges are as follows
- [ ] Implement feature rich source in a memory constrained environment (8KB Flash & 512B SRAM)
- [ ] Create a fully functioning development environment in Linux without using Atmel Studio 7 - This will involve using compilers, linkers, and debuggers via CL and/or utilizing VScode for a front end GUI while still leaving the underlying tools and technology open and configurable
- [ ] Implement a 128x64 pixel OLED driver without using off the shelf libraries (Both from a challenge perspective, and a memory constraint that may come in to play with small amounts of Flash and SRAM)
- [ ] Integrate the FATFS lib for interfacing an SD card connected over SPI
- [ ] Create a bootloader capable of loading a new image to Flash via UART or load a file from an SD card
- [ ] Implement a WD timer for the sake of trying it. (lol)
- [ ] Add low power capabilities
- [ ] Implement fancy addressable RGB LEDs over I2C
- [X] Implement a unit test framework (Unity & Ceedling)

## Hardware
For this project I will be using an [ATtiny87 from Atmel/Microchip](https://www.microchip.com/wwwproducts/en/ATTINY87). The part has a limited feature set and a small amount of memory, but hopefully enough to implement everything desired and provide an adequate challenge.
- 8KB of Flash
- 512B or SRAM
- 512B of EEPROM
- 1x UART
- 2x SPI
- 1x I2c

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

# Compile, Test and Flash
#### Compiling
To compile the application:
```
$ make app
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
$ make flash_app
```

To flash the bootloader .hex:
```
$ make flash_boot
```

