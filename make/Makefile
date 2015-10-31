###############################################################################
#                                                                             #
# Makefile                                                                    #
# ========                                                                    #
#                                                                             #
# Version: 1.4.4                                                              #
# Date   : 31.10.15                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/peterweissig/robolib                                   #
###############################################################################

# Target for compiling and linking (without extension)
# e.g. "main" for main.c
# If TARGET is empty ("") all source files (*.c) within the current folder
# will be used.
TARGET =

# Micro controller type
MCU = atmega2561

# Settings of the comport (e.g. "/dev/ttyUSB0" with "57600" baud) for
# exchanging data with robot over the python-script ("make data")
# If the name of the comport is set to "" the first comport in
# /dev/ttyUSB is automatically used. If none was found, the first
# comport matching /dev/ttyS* is used.
COMPORT_DATA  =
BAUDRATE_DATA = 57600

# Settings of the comport (e.g. "/dev/ttyUSB0" with "57600" baud) for
# downloading ("make program")
# If the name of the comport is set to "" the first comport in
# /dev/ttyUSB is automatically used. If none was found, the first
# comport matching /dev/ttyS* is used.
COMPORT_PROG  =
BAUDRATE_PROG = 57600

# Additional reset sequence for downloading ("make program")
# This reset sequence will be passed directly to the downloader.
# E.g. setting RTS and DTR - waiting 100ms - clearing RTS - waiting 100ms
#   "-R RTS+ -R DTR+ -R 100 -R RTS- -R 100"
RESET_SEQUENCE  =

# Settings of the comport (e.g. "/dev/ttyACM0" with "4" µs clocktime) for
# downloading via avrdude ("make avrdude")
# If SUDO_AVRDUDE is set avrdude is run with root privileges.
COMPORT_AVRDUDE  = /dev/ttyACM0
BAUDRATE_AVRDUDE = 4
SUDO_AVRDUDE =

# Directory of the used library (main-folder).
# If the path is set to "" the parent folder of this Makefile (../)
# will be used.
PATH_LIB =

# Startadress of code section in flash. This option is needed for bootloaders
# If Bootstart is set to "" the source code will start at standard
# address (0x000000).
BOOTSTART =

# Flag for using kate as an editor.
# OPTIONAL - If set the flags MAKE_PROGRAM_IN_NEW_TERMINAL and
# MAKE_DATA_IN_NEW_TERMINAL will be set.
KATE =

# Flag for using pololu-lib
# OPTIONAL - if set the project will also be linked against pololu-lib
POLOLU =

# Flag for opening a new console for the download ("make program").
# OPTIONAL - If set the downloader starts in a separate console.
MAKE_PROGRAM_IN_NEW_TERMINAL =

# Flag for opening a new console for the direct data exchange ("make data").
# OPTIONAL - If set the python-script starts in a separate console.
MAKE_DATA_IN_NEW_TERMINAL =

# Flag for debugging (extended output during processing)
# If not set ("") nothing changes.
# Otherwise additional output will be generated.
DEBUG =

###############################################################################
# additional identifiers
PATH_SUB_TMP  = tmp/
PATH_SUB_LIB  = lib/
PATH_SUB_DATA = data/

# list of file extensions for cleaning
FILES_CLEAN = *.a *.o *.hex *.obj *.elf *.lss *.lst *.map *.sym

# name of downloader
DOWNLOADER = $(PATH_LIB_INTERNAL)bin/downloader
# downloader options
DOWNLOADER_OPTIONS = -I $(OUTPUT).hex -v -p $(MCU)                            \
  -P $(COMPORT_PROG_INTERNAL) -b $(BAUDRATE_PROG) $(RESET_SEQUENCE)

# name of terminal
TERMINAL = python $(PATH_LIB_INTERNAL)bin/data.py
# terminal options
TERMINAL_OPTIONS = $(COMPORT_DATA_INTERNAL) $(BAUDRATE_DATA)

# name of downloader
ifeq ($(SUDO_AVRDUDE),)
  AVRDUDE = avrdude
else
  AVRDUDE = sudo avrdude
endif
# downloader options
AVRDUDE_OPTIONS = -v -p $(MCU) -P $(COMPORT_AVRDUDE) -c AVRISP2               \
  -B $(BAUDRATE_AVRDUDE) -e -U flash:w:$(OUTPUT).hex

###############################################################################
# flag control
ifneq (,$(KATE))
  MAKE_PROGRAM_IN_NEW_TERMINAL = 1
  MAKE_DATA_IN_NEW_TERMINAL = 1
endif

# get current directory (for opening a new console - ..._IN_NEW_TERMINAL)
PATH_CUR = $(shell pwd)/
ifneq ($(DEBUG),)
  $(info current dir: $(PATH_CUR))
endif

# get current directory of this makefile (for USE_LOCAL_LIBRARY)
PATH_MAKE = $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))/
ifneq ($(DEBUG),)
  $(info Makefile dir: $(PATH_MAKE))
endif

# special options - find a valid comport if none is given
ifeq (,$(COMPORT_PROG))
  COMPORT_PROG_INTERNAL = $(firstword $(wildcard /dev/ttyUSB*))
  ifeq (,$(COMPORT_PROG_INTERNAL))
    COMPORT_PROG_INTERNAL = $(firstword $(wildcard /dev/ttyS*))
  endif
else
  COMPORT_PROG_INTERNAL = $(COMPORT_PROG)
endif
ifneq ($(DEBUG),)
  $(info comport prog: $(COMPORT_PROG_INTERNAL))
endif

ifeq (,$(COMPORT_DATA))
  COMPORT_DATA_INTERNAL = $(firstword $(wildcard /dev/ttyUSB*))
  ifeq (,$(COMPORT_DATA_INTERNAL))
    COMPORT_DATA_INTERNAL = $(firstword $(wildcard /dev/ttyS*))
  endif
else
  COMPORT_DATA_INTERNAL = $(COMPORT_DATA)
endif
ifneq ($(DEBUG),)
  $(info comport data: $(COMPORT_DATA_INTERNAL))
endif

###############################################################################
# output, source-files, object-files
# name of output file
ifeq (,$(TARGET))
  OUTPUT = out
else
  OUTPUT = $(TARGET)
endif
ifneq ($(DEBUG),)
  $(info output file: $(OUTPUT))
endif

# source file(s)
ifneq (,$(TARGET))
  SRC = $(TARGET).c
else
  # main directory for sourcefiles
  PATH_SOURCE = ./
  # search for subdirectories
  PATH_SOURCE+= $(filter-out $(PATH_SUB_TMP) $(PATH_SUB_DATA)                 \
    $(PATH_SUB_LIB), $(filter %/,$(wildcard */)))

  # list of all source files within found paths
  SRC  = $(patsubst ./%,%,$(wildcard $(foreach dir,$(PATH_SOURCE), $(dir)*.c)))
endif
ifneq ($(DEBUG),)
  $(info source files: $(SRC))
endif

# list of object files
OBJ  = $(foreach source,$(basename $(SRC)),$(PATH_SUB_TMP)$(source).o)
ifneq ($(DEBUG),)
  $(info object files: $(OBJ))
endif

# libraries
ifeq (,$(PATH_LIB))
  PATH_LIB_INTERNAL = $(PATH_MAKE)../
else
  PATH_LIB_INTERNAL = $(PATH_LIB)
endif
ifneq ($(DEBUG),)
  $(info lib dir: $(PATH_LIB_INTERNAL))
endif

###############################################################################
# libraries and ini-files

# list of archives for "normal" project linking
ARCHIVES_NORMAL = $(wildcard $(PATH_SUB_LIB)*.a)


# list of files for "ini" compilation
INI_FILES = $(wildcard *.ini)
ifneq ($(strip $(INI_FILES)),)
  # additional compiler flags for including the main header(s)
  CFLAGS_INCLUDE_INI = $(foreach lib,$(basename $(notdir $(INI_FILES)))       \
    , -include $(lib).h)

  # all headers implied by the main header(s)
  HEADERS_INI = $(patsubst \#include"%",%,$(patsubst \#include<%>,%,          \
    $(filter \#include<%> \#include"%",$(strip $(shell cat $(INI_FILES))))))

  ifneq ($(strip $(HEADERS_INI)),)
    # all source files implied by the implied headers (HEADERS_INI)
    SOURCES_INI = $(foreach header,$(filter-out _%, $(basename $(notdir       \
      $(HEADERS_INI)))), $(PATH_LIB_INTERNAL)source/$(header)/$(header).c)
    SOURCES_INI+= $(foreach header,$(filter _%, $(basename $(notdir           \
      $(HEADERS_INI)))), $(PATH_LIB_INTERNAL)_source/$(header)/$(header).c)
    # all object files implied by the implied source files (SOURCES_INI)
    OBJECTS_INI = $(foreach source,$(basename $(notdir $(SOURCES_INI)))       \
      ,$(PATH_SUB_LIB)$(source).o)
  endif
endif

###############################################################################
# pololu

ifneq ($(strip $(POLOLU)),)
  POLOLU_LIB = $(PATH_MAKE)../../pololu/libpololu_$(MCU).a

  POLOLU_HEADER = $(PATH_MAKE)../../pololu/src/
  #POLOLU_HEADER = $(PATH_MAKE)../../pololu/pololu/
  #POLOLU_HEADER2 = $(PATH_MAKE)../../pololu/src/
endif

###############################################################################
# compiler flags

CFLAGS  = -mmcu=$(MCU) -I. -I "$(PATH_LIB_INTERNAL)"
CFLAGS += -gstabs -Os -funsigned-char -funsigned-bitfields
CFLAGS += -fpack-struct -fshort-enums
CFLAGS += -Wall -Wstrict-prototypes -std=gnu99
CFLAGS += -Wp,-M,-MP,-MT,$@
CFLAGS += -Wa,-adhlns=$(@:.o=.lst)
CFLAGS += -Wl,-gc-sections -Wl,-relax
LDFLAGS = -Wl,-Map=$(PATH_SUB_TMP)$(OUTPUT).map,--cref
ifneq ($(BOOTSTART),)
CFLAGS += -DBOOTSTART=$(BOOTSTART)
LDFLAGS+= -Ttext $(BOOTSTART)
endif

ifneq ($(POLOLU_HEADER),)
CFLAGS += -I "$(POLOLU_HEADER)"
endif

###############################################################################
# ignore implicit chain and therefore deletion of object files
.SECONDARY: $(OBJECTS_NORMAL)

# define phony targets for make commands
.PHONY: all all_init   ini ini_init   program avrdude   data   clean

all: all_init $(OUTPUT).hex
	@echo
	@avr-size -A $(PATH_SUB_TMP)$(OUTPUT).elf
	@echo
	@echo Compilation done - No Errors.

all_init:
	@echo
	@echo "#### compiling project \"make all\""
	@echo
	mkdir -p $(PATH_SUB_TMP)

%.hex: $(PATH_SUB_TMP)%.elf
	avr-objcopy -O ihex -R .eeprom $< $@
	avr-objdump -h -S $< > $(PATH_SUB_TMP)$*.lss

%.elf: $(OBJ) $(ARCHIVES_NORMAL)
	@echo
	@mkdir -p $(@D)
	@echo "#### linking project"
	avr-gcc $(CFLAGS) $(OBJ) $(ARCHIVES_NORMAL) --output $@ $(LDFLAGS)

$(PATH_SUB_TMP)%.o: %.c
	@echo
	@mkdir -p $(@D)
	@echo "#### compiling \"$@\" based on \"$^\""
	avr-gcc -o $@ -c $(CFLAGS) $(CFLAGS_INCLUDE_INI) -I "$(dir $*)/" $<

ini: ini_init $(OBJECTS_INI)
	@echo
ifneq ($(strip $(INI_FILES)),)
	@echo "#### creating library archive"
	avr-ar rs $(PATH_SUB_LIB)lib.a $(OBJECTS_INI)
	rm -f $(foreach file,$(filter-out *.a, $(FILES_CLEAN))                \
          , $(PATH_SUB_LIB)$(file))
	@echo
endif
	@echo "#### done :-)"
	@echo

ini_init:
	@echo
	@echo "#### initiating project \"make ini\""
	mkdir -p $(PATH_SUB_LIB)
	@echo
	@echo "clearing library-folder \"$(PATH_SUB_LIB)\""
	-rm -f -R $(PATH_SUB_LIB)*
	@echo
ifneq ($(strip $(INI_FILES)),)
	@echo "creating main header(s) \"$(INI_FILES:.ini=.h)\""
	@echo -e " $(foreach header,$(INI_FILES:.ini=)                        \
          , $(shell cp -f -v $(header).ini $(header).h)\n)"
	@echo
endif
ifneq ($(strip $(POLOLU_LIB)),)
	@echo "copying pololu lib \"$(basename $(POLOLU_LIB))\""
	@cp $(POLOLU_LIB) $(PATH_SUB_LIB)
	@echo
endif

$(PATH_SUB_LIB)%.o:
	@echo
	@echo "####" compiling \"$@\" based on \"$(filter %/$(basename        \
          $(notdir $@)).c, $(SOURCES_INI))\"
	avr-gcc -o $@ -c $(CFLAGS) $(CFLAGS_INCLUDE_INI) $(filter             \
          %/$(basename $(notdir $@)).c, $(SOURCES_INI))

clean:
	@echo
	@echo "####" cleaning project \"make clean\"
	-rm -f -R $(foreach file,$(FILES_CLEAN),$(PATH_SUB_LIB)$(file))
	-rm -f -R $(foreach file,$(FILES_CLEAN),$(PATH_SUB_TMP)$(file))
	-rm -f -R $(foreach file,$(FILES_CLEAN),$(PATH_SUB_TMP)*/$(file))

program: $(OUTPUT).hex
	@echo
	@echo "#### downloading project via downloader \"make program\""
ifeq (,$(MAKE_PROGRAM_IN_NEW_TERMINAL))
	$(DOWNLOADER) $(DOWNLOADER_OPTIONS)
else
	konsole -e bash -c 'cd $(PATH_CUR) && $(DOWNLOADER)                   \
          $(DOWNLOADER_OPTIONS); echo; echo "<press enter to exit>";          \
          read dummy'
endif

avrdude: $(OUTPUT).hex
	@echo
	@echo "#### downloading project via avrdude \"make avrdude\""
ifeq (,$(MAKE_PROGRAM_IN_NEW_TERMINAL))
	$(AVRDUDE) $(AVRDUDE_OPTIONS)
else
	konsole -e bash -c 'cd $(PATH_CUR) && $(AVRDUDE) $(AVRDUDE_OPTIONS);  \
          echo; echo "<press enter to exit>"; read dummy'
endif

data:
	@echo
	@echo "####" start python script for communication "make data"
	@mkdir -p $(PATH_SUB_DATA)
ifeq (,$(MAKE_DATA_IN_NEW_TERMINAL))
	@cd $(PATH_SUB_DATA) && $(TERMINAL) $(TERMINAL_OPTIONS)
	@cd $(PATH_CUR)
else
	konsole -e bash -c 'cd $(PATH_CUR)$(PATH_SUB_DATA) && $(TERMINAL)     \
          $(TERMINAL_OPTIONS)'
endif

###############################################################################
