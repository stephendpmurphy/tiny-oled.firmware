# attiny-oled
![CI - AVR-GCC Build](https://github.com/stephendpmurphy/tiny-oled.firmware/workflows/CI%20-%20AVR-GCC%20Build/badge.svg)
## Description
This project is intended to challenge myself and be used as a way to explore and try new concepts in a bare-metal 8bit environment. The current planned challenges are as follows
- [ ] Implement feature rich source in a memory constrained environment (8KB Flash & 512B SRAM)
- [X] Create a fully functioning development environment in Linux without using Atmel Studio 7 - This will involve using compilers, linkers, and debuggers via CL and/or utilizing VScode for a front end GUI while still leaving the underlying tools and technology open and configurable
- [ ] Implement a 128x64 pixel OLED driver without using off the shelf libraries (Both from a challenge perspective, and a memory constraint that may come in to play with small amounts of Flash and SRAM)
- [ ] Integrate the FATFS lib for interfacing an SD card connected over SPI
- [ ] Add a 9 axis Gyro sensor over SPI
- [ ] Add a temp and humidity sensor over SPI
- [ ] Create a bootloader capable of loading a new image to Flash via UART or load a file from an SD card
- [ ] Implement a WD timer for the sake of trying it.
- [ ] Add low power capabilities
- [X] Implement fancy addressable RGB LEDs
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

# Compile, Test, Flash and AVR Fuses
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

#### Reading & Writing Fuses
Fuses are a set of configuration bytes in all AVR hardware that tell the chip things like what clock input and divider to use, enabling brown-out detection, enabling the Watchdog, and a few other features. The attiny87/167 has an internal 8Mhz clock that is then divided down by 8, to give a CPU clock of 1Mhz out of the box. We want to run the part faster however, so we need to disable the Clock divider bit in the lower fuse (lfuse) to give us a CPU clock of 8Mhz. You can calculate the correct value of the fuses using the [AVR Fuse Calc](https://www.engbedded.com/fusecalc/) or simply reference the Fuse section of the datasheet and calculate the correct value yourself.

First read the device fuses. The end portion following **-U** says we want to access the **lfuse** by executing a **read (r)** and store the output in a text file called **lfuse.txt** and the format should be in **hexadecimal (h)**
```
$ avrdude -p attiny87 -c avrispmkII -U lfuse:r:lfuse.txt:h
```

The default value for the lfuse in an attiny87 should be **0x62**. The logic on fuses is that a bit is **UNPROGRAMMED** if it is a 1, and **PROGRAMMED** if it is a 0. We need to unprogram the CKDIV8 bit (bit 7), by changing it to a 1. This gives us a new lfuse hex value of **0xe2**.

We can write this new fuse value with the following
```
$ avrdude -p attiny87 -c avrispmkII -U lfuse:w:0xe2:m
```

All done! Your device should now be clocked at the appropriate 8Mhz. If you decide to use an external oscillator or clock the device with a different prescaler, be sure to update the **F_CPU** macro in the Makefile to match your desired clock frequency.
```
# Macro Definitions
F_CPU=8000000
```

To simplify things. You can execute the **write_fuses** option with **make** to write the appropriate fuse values for this project.
```
$ make write_fuses
```

And you can execute the **read_fuses** option with **make** to read out the lfuse, hfuse and efuse values into their respective .txt files
```
$ make read_fuses
```
