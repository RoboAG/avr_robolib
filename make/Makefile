###############################################################################
#                                                                             #
# Makefile                                                                    #
# ========                                                                    #
#                                                                             #
# Version: 1.7.2                                                              #
# Date   : 19.02.2022                                                         #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/RoboAG/avr_robolib                                     #
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

# Flag for turning off the auto loading of *.make files.
# If not set (""), this Makefile looks for a *.make file and calls itself
# with all parameters from the *.make files.
NO_AUTO =

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
PATH_MAKE:= $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))/
ifneq ($(DEBUG),)
    $(info Makefile dir: $(PATH_MAKE))
endif

# get working directory (for opening a new console - ..._IN_NEW_TERMINAL)
PATH_CUR:= $(shell pwd)/
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
# auto loading of *.make

ifeq ($(NO_AUTO),)
    ifneq ($(wildcard *.mcu),)
        AUTOLOAD_FILE:= $(firstword $(wildcard *.mcu))
        AUTOLOAD_MCU  = $(patsubst %.mcu, %, $(AUTOLOAD_FILE))
        AUTOLOAD_ARGS:= $(shell cat $(AUTOLOAD_FILE))
        ifneq ($(DEBUG),)
            $(info auto-loading File: $(AUTOLOAD_FILE))
            $(info auto-loading MCU : $(AUTOLOAD_MCU))
            $(info auto-loading ARGS: $(AUTOLOAD_ARGS))
        endif
        include $(PATH_CUR)$(AUTOLOAD_FILE)
        MCU = $(AUTOLOAD_MCU)
    endif
endif

###############################################################################
# check for special MCU types

ifeq ($(strip $(MCU)),3pi)
    MCU_TEMP = atmega328p
    POLOLU = 1
else
    MCU_TEMP = $(MCU)
endif
MCU_INTERNAL=$(strip $(MCU_TEMP))

###############################################################################
# name of downloader
DOWNLOADER = $(PATH_ROBOLIB_INTERNAL)$(DOWNLOADER_BIN)
# downloader options
DOWNLOADER_OPTIONS = -I $(OUTPUT).hex -v -p $(MCU_INTERNAL) \
  -P $(COMPORT_PROG_INTERNAL) -b $(BAUDRATE_PROG) $(RESET_SEQUENCE)

# name of terminal
DATA = python $(PATH_ROBOLIB_INTERNAL)$(DATA_BIN)
# terminal options
DATA_OPTIONS = "$(COMPORT_DATA_INTERNAL)" "$(BAUDRATE_DATA)"

# name of downloader (avrdude)
ifeq ($(SUDO_AVRDUDE),)
    AVRDUDE = avrdude
else
    AVRDUDE = sudo avrdude
endif
# downloader options
AVRDUDE_OPTIONS = -v -p $(MCU_INTERNAL) -P $(COMPORT_AVRDUDE) -c AVRISP2 \
  -B $(BAUDRATE_AVRDUDE) -e -U flash:w:$(OUTPUT).hex

###############################################################################
# flag control

# check for terminal (for opening a new console - ..._IN_NEW_TERMINAL)
ifneq (,$(FORCE_TERMINAL))
    TERM:= "$(FORCE_TERMINAL)"
else
    ifeq (,$(KATE))
        TERM_INTERNAL = no terminal needed
    endif
    ifeq (,$(TERM_INTERNAL))
        ifneq (,$(shell which gnome-terminal))
            TERM_INTERNAL = gnome-terminal
        else
            ifneq (,$(shell which xfce4-terminal))
                TERM_INTERNAL = xfce4-terminal
            else
                ifneq (,$(shell which konsole))
                    TERM_INTERNAL = konsole
                else
                    TERM_INTERNAL = xterm
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
    COMPORT_PROG_INTERNAL:= $(firstword $(wildcard /dev/ttyUSB*))
    ifeq (,$(COMPORT_PROG_INTERNAL))
        COMPORT_PROG_INTERNAL:= $(firstword $(wildcard /dev/ttyS*))
    endif
else
    COMPORT_PROG_INTERNAL:= $(COMPORT_PROG)
endif
ifneq ($(DEBUG),)
    $(info comport prog: $(COMPORT_PROG_INTERNAL))
endif

ifeq (,$(COMPORT_DATA))
    COMPORT_DATA_INTERNAL:= $(firstword $(wildcard /dev/ttyUSB*))
    ifeq (,$(COMPORT_DATA_INTERNAL))
        COMPORT_DATA_INTERNAL:= $(firstword $(wildcard /dev/ttyS*))
    endif
else
    COMPORT_DATA_INTERNAL:= $(COMPORT_DATA)
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
    PATH_SOURCE:= ./
    # search for subdirectories
    PATH_SOURCE+= $(filter-out $(PATH_SUB_TMP) $(PATH_SUB_DATA) \
      $(PATH_SUB_LIB), $(filter %/,$(wildcard */)))

    # list of all source files within found paths
    SRC:= $(patsubst ./%,%,$(wildcard \
      $(foreach dir,$(PATH_SOURCE), $(dir)*.c)))
endif
ifneq ($(DEBUG),)
    $(info source files: $(SRC))
endif

# list of object files
OBJ:= $(foreach source,$(basename $(SRC)),$(PATH_SUB_TMP)$(source).o)
ifneq ($(DEBUG),)
    $(info object files: $(OBJ))
endif

###############################################################################
# archives (libraries)

# list of files for "ini" compilation
INI_FILES:= $(wildcard *.ini)
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
    LIB_POLOLU:= $(PATH_MAKE)../../pololu/libpololu_$(MCU_INTERNAL).a

    HDR_POLOLU:= $(PATH_MAKE)../../pololu/src/
    #HDR_POLOLU = $(PATH_MAKE)../../pololu/pololu/
endif

# archives before
LIB_BEFORE:= $(filter-out $(LIB_INI),$(wildcard $(PATH_SUB_LIB)*.a))

# list of all archives
LIB_ALL:= $(sort $(LIB_BEFORE) $(LIB_INI) $(LIB_POLOLU))
ifneq ($(DEBUG),)
    $(info all libs: $(LIB_ALL))
endif

###############################################################################
# compiler and linker flags

CFLAGS  = -mmcu=$(MCU_INTERNAL) -I. -I "$(PATH_ROBOLIB_INC)"
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

ifneq ($(HDR_POLOLU),)
    CFLAGS += -I "$(HDR_POLOLU)"
endif

CFLAGS_INCLUDE_INI:= $(foreach hdr,$(HDR_INI),-include "$(hdr)")

###############################################################################
# ignore implicit chain and therefore deletion of hex-file and elf-file
.SECONDARY: $(OUTPUT).hex $(PATH_SUB_TMP)$(OUTPUT).elf

# allow second expansion for complex rules
.SECONDEXPANSION:
# wrap symbols
PERCENT=%
HASHTAG=\#
QUOTE="
COLON=:

# define phony targets for make commands
.PHONY:   build all   lib lib_init   compile compile_init \
          clean_keep_output clean                         \
          init_kepler init_3pi  \
          program download avrdude   data


build: lib compile
	@echo
	@echo "All done :-)"

all: build
	@echo
	@echo "'make all' is deprecated - please use **make build** instead"


lib: lib_init $(LIB_INI) $(HDR_INI)

lib_init:
	@echo
	@echo "*** initiating project \"make lib\""
	mkdir -p $(PATH_SUB_LIB)


compile: compile_init $(OUTPUT).hex
	@echo
	@echo Compilation done - No Errors.

compile_init:
	@echo
	@echo "*** compiling project \"make compile\""
	@echo
	mkdir -p $(PATH_SUB_TMP)


clean_keep_output:
	@echo
	@echo "*** cleaning project \"make clean...\""
	-rm -f -R $(PATH_SUB_LIB)
	-rm -f -R $(PATH_SUB_TMP)
	-rm -f -R $(filter-out *.hex, $(FILES_CLEAN))
	-rm -f -R $(HDR_INI)

clean: clean_keep_output
	-rm -f -R *.hex


init_kepler:
	@echo
	@echo "*** initialize for keplerboard \"make $@\""
ifeq ($(wildcard *.mcu),)
	@echo "# Comports"                           >  atmega2561.mcu
	@echo "#COMPORT_DATA=/dev/ttyUSB0"           >> atmega2561.mcu
	@echo "#COMPORT_PROG=/dev/ttyS0"             >> atmega2561.mcu
	@echo ""                                     >> atmega2561.mcu
	@echo "# Reset sequence for old Keplerboard" >> atmega2561.mcu
	@echo "#RESET_SEQUENCE=-R RTS+ -R DTR+ -R 100 -R RTS- -R 200" \
	  >> atmega2561.mcu
else
	@echo "error mcu-file already exist"
	@echo "  ($(wildcard *.mcu))"
	@false
endif

init_3pi:
	@echo
	@echo "*** initialize for 3pi \"make $@\""
ifeq ($(wildcard *.mcu),)
	@echo "# Comports"                           >  3pi.mcu
	@echo "#COMPORT_AVRDUDE=/dev/ttyACM0"        >> 3pi.mcu
	@echo "#COMPORT_DATA=/dev/ttyUSB0"           >> 3pi.mcu
else
	@echo "error mcu-file already exist"
	@echo "  ($(wildcard *.mcu))"
	@false
endif
ifeq ($(wildcard *.c),)
	@echo "// include from pololu"               >   main.c
	@echo "#include <Pololu3pi/Pololu3pi.h>"     >>  main.c
	@echo ""                                     >>  main.c
	@echo "int main(void) {"                     >>  main.c
	@echo ""                                     >>  main.c
	@echo "    // initialize system"             >>  main.c
	@echo "    set_motors(0,0);"                 >>  main.c
	@echo ""                                     >>  main.c
	@echo "    // draw a smiley on the Display"  >>  main.c
	@echo "    clear();"                         >>  main.c
	@echo "    print(\"  :-)\");"                >>  main.c
	@echo ""                                     >>  main.c
	@echo "    // turn on red led"               >>  main.c
	@echo "    red_led(HIGH);"                   >>  main.c
	@echo ""                                     >>  main.c
	@echo "    // endless loop"                  >>  main.c
	@echo "    while (1) {"                      >>  main.c
	@echo "        // toogle green led"          >>  main.c
	@echo "        green_led(TOGGLE);"           >>  main.c
	@echo "        delay_ms(250);"               >>  main.c
	@echo "    }"                                >>  main.c
	@echo ""                                     >>  main.c
	@echo "    return 0;"                        >>  main.c
	@echo "}"                                    >>  main.c
	@echo ""                                     >>  main.c

else
	@echo "error c-file already exist"
	@echo "  ($(wildcard *.c))"
	@false
endif


# only for backwards compatibility
program: download

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
	$(TERM_INTERNAL) -e "bash -c 'cd $(PATH_CUR) && \
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
	@echo
	@avr-size --mcu $(MCU_INTERNAL) -C $@

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
	@echo "** compress archiv \"$@\""
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
