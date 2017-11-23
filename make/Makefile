###############################################################################
#                                                                             #
# Makefile                                                                    #
# ========                                                                    #
#                                                                             #
# Version: 1.5.4                                                              #
# Date   : 23.11.17                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/peterweissig/robolib                                   #
###############################################################################

# Target for compiling and linking (without extension)
# e.g. "main" for main.c
# If TARGET is empty (""), all source files (*.c) within the current folder
# will be used.
TARGET =

# Micro controller type
MCU = atmega2561

# Settings of the comport (e.g. "/dev/ttyUSB0" with "57600" baud) for
# exchanging data with robot over the python-script ("make data")
# If the name of the comport is set to "", the first comport in
# /dev/ttyUSB is automatically used. If none was found, the first
# comport matching /dev/ttyS* is used.
COMPORT_DATA  =
BAUDRATE_DATA = 57600

# Settings of the comport (e.g. "/dev/ttyUSB0" with "57600" baud) for
# downloading ("make program")
# If the name of the comport is set to "", the first comport in
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
# If SUDO_AVRDUDE is set, avrdude is run with root privileges.
COMPORT_AVRDUDE  = /dev/ttyACM0
BAUDRATE_AVRDUDE = 4
SUDO_AVRDUDE =

# Directory of the used library (main-folder).
# If the path is set to "", the parent folder of this Makefile (../)
# will be used.
PATH_ROBOLIB =

# Startadress of code section in flash. This option is needed for bootloaders
# If Bootstart is set to "", the source code will start at standard
# address (0x000000).
BOOTSTART =

# Flag for using kate as an editor.
# OPTIONAL - If set, the downloader and the python script start in a
# seperat terminal.
KATE =

# Flag for using pololu-lib
# OPTIONAL - If set, the project will also be linked against pololu-lib
POLOLU =

# Program to be used as terminal (see KATE).
# OPTIONAL - If not set, the following terminals will be checked and used
#   1. gnome-terminal
#   2. xfce4-terminal
#   3. konsole
#   4. xterm
FORCE_TERMINAL =

# Flag for debugging (extended output during processing)
# If not set ("") nothing changes.
# Otherwise additional output will be generated.
DEBUG =

###############################################################################
# local directories
PATH_SUB_TMP  = tmp/
PATH_SUB_LIB  = lib/
PATH_SUB_DATA = data/

# robolib directories
PATH_SUB_INC  = include/
PATH_SUB_SRC  = src/robolib/

# list of file extensions for cleaning
FILES_CLEAN = *.a *.o *.hex *.obj *.elf *.lss *.lst *.map *.sym

# programs
DOWNLOADER_BIN = bin/downloader
DATA_BIN       = bin/data.py

###############################################################################
# important paths

# get current directory of this makefile
PATH_MAKE = $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))/
ifneq ($(DEBUG),)
    $(info Makefile dir: $(PATH_MAKE))
endif

# get working directory (for opening a new console - ..._IN_NEW_TERMINAL)
PATH_CUR = $(shell pwd)/
ifneq ($(DEBUG),)
    $(info current dir: $(PATH_CUR))
endif

# get directory of robolib
ifeq (,$(PATH_ROBOLIB))
    PATH_ROBOLIB_INTERNAL = $(patsubst %/,%,$(abspath $(PATH_MAKE)../))/
else
    PATH_ROBOLIB_INTERNAL = $(patsubst %/,%,$(abspath $(PATH_ROBOLIB)))/
endif
ifneq ($(DEBUG),)
    $(info robolib dir: $(PATH_ROBOLIB_INTERNAL))
endif

# set subdirectories of robolib
PATH_ROBOLIB_INC = $(PATH_ROBOLIB_INTERNAL)$(PATH_SUB_INC)
PATH_ROBOLIB_SRC = $(PATH_ROBOLIB_INTERNAL)$(PATH_SUB_SRC)

###############################################################################
# name of downloader
DOWNLOADER = $(PATH_ROBOLIB_INTERNAL)$(DOWNLOADER_BIN)
# downloader options
DOWNLOADER_OPTIONS = -I $(OUTPUT).hex -v -p $(MCU) \
  -P $(COMPORT_PROG_INTERNAL) -b $(BAUDRATE_PROG) $(RESET_SEQUENCE)

# name of terminal
DATA = python $(PATH_ROBOLIB_INTERNAL)$(DATA_BIN)
# terminal options
DATA_OPTIONS = $(COMPORT_DATA_INTERNAL) $(BAUDRATE_DATA)

# name of downloader (avrdude)
ifeq ($(SUDO_AVRDUDE),)
    AVRDUDE = avrdude
else
    AVRDUDE = sudo avrdude
endif
# downloader options
AVRDUDE_OPTIONS = -v -p $(MCU) -P $(COMPORT_AVRDUDE) -c AVRISP2 \
  -B $(BAUDRATE_AVRDUDE) -e -U flash:w:$(OUTPUT).hex

###############################################################################
# flag control

# check for terminal (for opening a new console - ..._IN_NEW_TERMINAL)
ifneq (,$(FORCE_TERMINAL))
    TERM = "$(FORCE_TERMINAL)"
else
    ifeq (,$(KATE))
        TERM_INTERNAL= no terminal needed
    endif
    ifeq (,$(TERM_INTERNAL))
        ifneq (,$(shell which gnome-terminal))
            TERM_INTERNAL= gnome-terminal
        else
            ifneq (,$(shell which xfce4-terminal))
                TERM_INTERNAL= xfce4-terminal
            else
                ifneq (,$(shell which konsole))
                    TERM_INTERNAL= konsole
                else
                    TERM_INTERNAL= xterm
                endif
            endif
        endif
    endif
endif
ifneq ($(DEBUG),)
    $(info terminal: $(TERM_INTERNAL))
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
    PATH_SOURCE+= $(filter-out $(PATH_SUB_TMP) $(PATH_SUB_DATA) \
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

###############################################################################
# archives (libraries)

# list of files for "ini" compilation
INI_FILES = $(wildcard *.ini)
ifneq ($(DEBUG),)
    $(info ini files: $(INI_FILES))
endif

# list of headers and libraries to be created by "ini" compilation
HDR_INI = $(INI_FILES:.ini=.h)
LIB_INI = $(foreach lib,$(INI_FILES:.ini=.a),$(PATH_SUB_LIB)$(lib))
ifneq ($(DEBUG),)
    $(info libs from ini: $(LIB_INI))
endif

# pololu
ifneq ($(strip $(POLOLU)),)
    LIB_POLOLU = $(PATH_MAKE)../../pololu/libpololu_$(MCU).a

    HDR_POLOLU = $(PATH_MAKE)../../pololu/src/
    #HDR_POLOLU = $(PATH_MAKE)../../pololu/pololu/
endif

# archives before
LIB_BEFORE = $(filter-out $(LIB_INI),$(wildcard $(PATH_SUB_LIB)*.a))

# list of all archives
LIB_ALL = $(sort $(LIB_BEFORE) $(LIB_INI) $(LIB_POLOLU))
ifneq ($(DEBUG),)
    $(info all libs: $(LIB_ALL))
endif

###############################################################################
# compiler and linker flags

CFLAGS  = -mmcu=$(MCU) -I. -I "$(PATH_ROBOLIB_INC)"
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

CFLAGS_INCLUDE_INI = $(foreach hdr,$(HDR_INI),-include "$(hdr)")

###############################################################################
# ignore implicit chain and therefore deletion of object files
#.SECONDARY: $(OBJECTS_NORMAL)

# allow second expansion for complex rules
.SECONDEXPANSION:
# wrap symbols
PERCENT=%
HASHTAG=\#
QUOTE="
COLON=:

# define phony targets for make commands
.PHONY: all all_init   ini ini_init   program download avrdude   data \
  clean clean_to_store

all: all_init ini $(OUTPUT).hex
	@echo
	@avr-size -A $(PATH_SUB_TMP)$(OUTPUT).elf
	@echo
	@echo Compilation done - No Errors.

all_init:
	@echo
	@echo "*** compiling project \"make all\""
	@echo
	mkdir -p $(PATH_SUB_TMP)

ini: ini_init $(LIB_INI) $(HDR_INI)
	@echo
	@echo "** done :-)"

ini_init:
	@echo
	@echo "*** initiating project \"make ini\""
	mkdir -p $(PATH_SUB_LIB)

clean:
	@echo
	@echo "*** cleaning project \"make $@\""
	-rm -f -R $(PATH_SUB_TMP)
	-rm -f -R $(foreach file,$(FILES_CLEAN),$(file))

clean_to_store:
	@echo
	@echo "*** cleaning project \"make $@\""
	-rm -f -R $(PATH_SUB_LIB)
	-rm -f -R $(PATH_SUB_TMP)
	-rm -f -R $(foreach file,$(filter-out *.hex, $(FILES_CLEAN)),$(file))
	-rm -f -R $(HDR_INI)

program: $(OUTPUT).hex

download:
	@echo
	@echo "*** downloading project via downloader \"make $@\""
ifeq (,$(KATE))
	$(DOWNLOADER) $(DOWNLOADER_OPTIONS)
else
	$(TERM_INTERNAL) -e "bash -c 'cd $(PATH_CUR) && \
          $(DOWNLOADER) $(DOWNLOADER_OPTIONS); \
          echo; echo \"<press enter to exit>\"; read dummy'"
endif

avrdude:
	@echo
	@echo "*** downloading project via avrdude \"make $@\""
ifeq (,$(KATE))
	$(AVRDUDE) $(AVRDUDE_OPTIONS)
else
	$(TERM_INTERNAL) -e bash "-c 'cd $(PATH_CUR) && \
          $(AVRDUDE) $(AVRDUDE_OPTIONS);  \
          echo; echo \"<press enter to exit>\"; read dummy'"
endif

data:
	@echo
	@echo "*** start python script for communication \"make $@\""
	@mkdir -p $(PATH_SUB_DATA)
ifeq (,$(KATE))
	@cd $(PATH_SUB_DATA) && $(DATA) $(DATA_OPTIONS)
	@cd $(PATH_CUR)
else
	$(TERM_INTERNAL) -e "bash -c 'cd $(PATH_CUR)$(PATH_SUB_DATA) && \
          $(DATA) $(DATA_OPTIONS)'"
endif

# main path
%.hex: $(PATH_SUB_TMP)%.elf
	avr-objcopy -O ihex -R .eeprom $< $@
	avr-objdump -h -S $< > $(PATH_SUB_TMP)$*.lss

%.h: %.ini
	@echo
	@echo "** creating \"$@\" based on \"$<\""
	cp $< $@

# subpath tmp
%.elf: $(OBJ) $(LIB_BEFORE)
	@echo
	@mkdir -p $(@D)
	@echo "*** linking project"
	avr-gcc $(CFLAGS) $(OBJ) $(LIB_ALL) --output $@ $(LDFLAGS)

$(PATH_SUB_TMP)%.o: %.c
	@echo
	@mkdir -p $(@D)
	@echo "** compiling \"$@\" based on \"$^\""
	avr-gcc -o $@ -c $(CFLAGS) -I "$(dir $*)/" $<


# subpath lib
call_prerequisite_dot_a = $(foreach hdr,$(patsubst robolib/$(PERCENT), \
  $(PERCENT), $(patsubst $(HASHTAG)include$(QUOTE)$(PERCENT)$(QUOTE), \
  $(PERCENT), $(patsubst $(HASHTAG)include<$(PERCENT)>, \
  $(PERCENT), $(filter $(HASHTAG)include<$(PERCENT)> \
  $(HASHTAG)include$(QUOTE)$(PERCENT)$(QUOTE), \
  $(shell cat $*.ini))))), $(PATH_SUB_LIB)$(hdr$(COLON).h=.o))
$(PATH_SUB_LIB)%.a: %.h $$(call call_prerequisite_dot_a)
	@echo
	@echo "*** compress archiv \"$@\""
	avr-ar rs $@ $(filter-out $<,$?)
	rm -f $(foreach file,$(filter-out *.a, $(FILES_CLEAN)) \
	  , $(PATH_SUB_LIB)$(file))

call_prerequisite_dot_o = $(wildcard $(PATH_ROBOLIB_SRC)$(basename $*)/*.c)
$(PATH_SUB_LIB)%.o: $$(call call_prerequisite_dot_o)
	@echo
	@echo "** compiling \"$@\" based on $(PATH_SUB_SRC)$*/$*.c"
	@mkdir -p $(@D)
	avr-gcc -o $@ -c $(CFLAGS) $(filter %$*.c,$?) \
	  $(CFLAGS_INCLUDE_INI) -I $(PATH_ROBOLIB_SRC)

###############################################################################
