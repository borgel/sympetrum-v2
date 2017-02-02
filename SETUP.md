# OSX
## CubeMX

A very handy GUI tool for confguring peripherals, clocktrees, etc. Can generate init code. Figuring out how to init peripherals in ST land can be _murderous_, so I recommend trying to use the tool.

1. Download STM32 Cube MX

2. Install it. This will REQURE a version of the JRE and JDK (1.8 at time of writing). You MUST install these from the Oracle website [1] before the CubeMX installer will run.

3. Run the CubeMX installer. By default it installs a `/Applications/STMicroelectronics/STM32CubeMX.app`.

4. Run CubeMX and start a new project to configure a board.

5. When running the code generator, you will be asked to choose a toolchain. Unfortunately none of the options are "gcc" or similar. I've heard it recommended to select TrueStudio to generate our linker script for us. We will also need to manually construct a `makefile`.


### Notes

* The first time you run the code generator, it will notice you're missing the STM32Cube libraries themselves and offer to download them. Do this.
* There is a tool to generate Makefiles from CubeMX projects [6], but I was unable to get it to work. (However I did end up using their makefile as a template).

## Toolchain

1. Install the latest GNU Embedded Toolchain with `brew cask install gcc-arm-embedded`

2. Alternativel install latest GNU Embedded Toolchain for ARM from [3]. One recommendation for this is to do

```
$ mkdir -p /usr/local
$ cd /usr/local
$ sudo tar xjf ~/Downloads/gcc-arm-none-eabi-4_9-2015q3-20150921-mac.tar.bz2
```

3. Install the programming tools via Homebrew (or from source[4]) `brew install stlink`. A small tutorial for using these tools to flash a board is at [5].

## Building

1. TODO explain how to adapt my Makefile.

## Flashing

1. TODO. Explain how to call `st-flash write FILENAME.bin 0x8000000` and what each param means.

# Other Platforms

For Windows and Linux, apparently there is now a free (not open source?) Eclipse-based IDE for STM32 [2].



[1] http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html http://www.oracle.com/technetwork/java/javase/downloads/jre8-downloads-2133155.html
[2] http://www.st.com/en/development-tools/sw4stm32.html http://www.openstm32.org/HomePage
[3] https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
[4] https://github.com/texane/stlink
[5] https://github.com/texane/stlink/blob/master/doc/tutorial.md
[6] https://github.com/baoshi/CubeMX2Makefile

