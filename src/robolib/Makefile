###############################################################################
#                                                                             #
# src/robolib/Makefile                                                        #
# ====================                                                        #
#                                                                             #
# Version: 1.0.1                                                              #
# Date   : 15.11.17                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/peterweissig/robolib                                   #
###############################################################################

# global paths
PATH_INCLUDE = ../../include/
PATH_TEMP    = ../../tmp/

# specific paths
PATH_OUT     = $(PATH_TEMP)robolib/

###############################################################################
# search for sources
SOURCES  = $(patsubst %/,%,$(wildcard */))
SOURCE_FILES = $(foreach source, $(SOURCES),$(source)/$(source).c)

# search for headers
HEADERS = $(wildcard $(PATH_INCLUDE)robolib/*.h)

# create directly given ojects
OBJS_REGULAR = $(foreach source, $(SOURCES), $(foreach mcu,$(patsubst \
  $(source)_%.c,%, $(notdir $(wildcard $(source)/$(source)_*.c))), \
  $(PATH_OUT)$(mcu)/$(source).o))
# create all other ojects (mcu independend source)
MCUS = $(sort $(foreach obj, $(OBJS_REGULAR), \
  $(patsubst $(PATH_OUT)%/,%,$(dir $(obj)))))
OBJS_OTHER = $(foreach source, $(filter-out \
  $(basename $(notdir $(OBJS_REGULAR))),$(SOURCES)), \
  $(foreach mcu,$(MCUS), $(PATH_OUT)$(mcu)/$(source).o))
# all objs
OBJS = $(OBJS_REGULAR) $(OBJS_OTHER)

# all archive files
ARCHIVES = $(patsubst %,$(PATH_OUT)%/lib.a,$(MCUS))

###############################################################################
# Compiler flags
#CFLAGS = -mmcu=$(MCU)
CFLAGS  = -gstabs -lm
CFLAGS += -Os
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -Wall -Wstrict-prototypes -Wa,-adhlns=$(<:.c=.lst)
CFLAGS += -I. -I "$(PATH_INCLUDE)" -I /usr/lib/avr
CFLAGS += -std=gnu99

###############################################################################
# define phony targets for make commands
.PHONY: all all_init clean
# secondary expansion to allow usage of more complex rules
.SECONDEXPANSION:
# wrap percentage symbol
PERCENT=%

all: all_init $(ARCHIVES)


all_init:
	@echo
	@echo "### compiling robolib ###"


clean:
	@echo
	@echo "### clean robolib ###"
	rm -f *.o *.hex *.obj *.elf *.lss *.lst *.map *.sym
	rm -rf $(PATH_OUT)


$(PATH_OUT)%/lib.a: $$(filter $(PATH_OUT)$$*/$$(PERCENT),$(OBJS))
	@echo
	@echo "Creating library for $*"
	@echo "  $<"
	-avr-ar rs $@ $<
	-rm -f *.lst

$(PATH_OUT)%.o: $$(wildcard $$(notdir $$*)/*.c)
	@echo
	@echo "Compiling $@:"
	avr-gcc -c $(CFLAGS) -mmcu=$(patsubst %/,%,$(dir $*)) -c $<  -o $@


# $(PATH_OUT)%.o: $$(notdir $$*)/$$(notdir $$*).c
# 	@echo
# 	@echo "Compiling $@:"

###############################################################################
