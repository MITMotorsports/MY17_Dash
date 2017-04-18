### DISCLAIMER
### This is an example Makefile and it MUST be configured to suit your needs.
### For detailled explanations about all the avalaible options,
### please refer to https://github.com/sudar/Arduino-Makefile/blob/master/arduino-mk-vars.md

UNAME := $(shell uname -s)

ifeq ($(UNAME), Linux)
	PROJECT_DIR 			:= $(abspath $(abspath $(lastword $(MAKEFILE_LIST)))/../../..)
	ARDUINO_DIR       = /usr/share/arduino
	BOARD_TAG         = mega2560
	AVRDUDE          := $(shell which avrdude)
	MONITOR_PORT      = /dev/ttyUSB*
endif

ifeq ($(UNAME), Darwin)
	PROJECT_DIR 			:= $(realpath $(abspath $(lastword $(MAKEFILE_LIST)))/../../..)
	ARDUINO_DIR       = /Applications/Arduino.app/Contents/Java
	BOARD_TAG         = mega
	BOARD_SUB         = atmega2560
	AVRDUDE          = /usr/local/CrossPack-AVR/bin/avrdude
	MONITOR_PORT      = /dev/tty.usbserial*
endif
### PROJECT_DIR
### This is the path to where you have created/cloned your project

### AVR_GCC_VERSION
### Check if the version is equal or higher than 4.9
AVR_GCC_VERSION  := $(shell expr `avr-gcc -dumpversion | cut -f1` \>= 4.9)

### ARDMK_DIR
### Path to the Arduino-Makefile directory.
ARDMK_DIR         = $(PROJECT_DIR)/Arduino-Makefile

### USER_LIB_PATH
### Path to where the your project's libraries are stored.
USER_LIB_PATH     :=  $(realpath $(PROJECT_DIR)/lib)

### MONITOR_BAUDRATE
### It must be set to Serial baudrate value you are using.
MONITOR_BAUDRATE  = 115200

### AVR_TOOLS_DIR
### Path to the AVR tools directory such as avr-gcc, avr-g++, etc.
AVR_TOOLS_DIR     := $(abspath $(shell which avr-g++)/../..)

### CFLAGS_STD
CFLAGS_STD        = -std=gnu11

### CXXFLAGS_STD
CXXFLAGS_STD      = -std=gnu++11 -DCAN_ARCHITECTURE_AVR

### CPPFLAGS
### Flags you might want to set for debugging purpose. Comment to stop.
CXXFLAGS         = -Wall

### If avr-gcc -v is higher than 4.9, activate coloring of the output
ifeq "$(AVR_GCC_VERSION)" "1"
    CXXFLAGS += -fdiagnostics-color
endif

### OBJDIR
### This is were you put the binaries you just compile using 'make'
CURRENT_DIR       = $(shell basename $(CURDIR))
OBJDIR            = $(PROJECT_DIR)/bin/$(CURRENT_DIR)/$(BOARD_TAG)

### path to Arduino.mk, inside the ARDMK_DIR, don't touch.
include $(ARDMK_DIR)/Arduino.mk
