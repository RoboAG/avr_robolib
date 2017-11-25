###############################################################################
#                                                                             #
# src/examples/Makefile                                                       #
# =====================                                                       #
#                                                                             #
# Version: 1.1.1                                                              #
# Date   : 23.11.17                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/peterweissig/robolib                                   #
###############################################################################

# global paths
MAKEFILE  = ../../../../make/Makefile
PATH_TEMP = ../../tmp/

# specific paths
PATH_OUT  = $(PATH_TEMP)examples/

# suffix of files to be copied
SUFFIX = %.c %.ini

###############################################################################
# search for sources (of examples)
EXAMPLES  = $(patsubst %/,%,$(wildcard */))
SRC_FILES_EX = $(foreach source, $(EXAMPLES),$(source)/$(source).c)

# create hex-files based on filename and mcu-type
HEX_FILES_EX = $(foreach source, $(EXAMPLES), $(foreach mcu,$(shell cat \
  $(source)/mcu.txt), $(PATH_OUT)$(mcu)/$(source)/out.hex))


# list copied c-files and ini-files (for SECONDARY)
SOURCE_FILES_COPY = $(foreach hex, $(HEX_FILES_EX), $(dir $(hex))$(notdir \
  $(patsubst %/,%,$(dir $(hex)))).c)
INI_FILES_COPY    = $(foreach hex, $(HEX_FILES_EX), $(dir $(hex))$(notdir \
  $(patsubst %/,%,$(dir $(hex)))).ini)


# search for sources and headers of library (for dependencies)
SOURCES_LIB = $(wildcard */*.c)
HEADERS_LIB = $(wildcard $(PATH_INCLUDE)robolib/*.h)

###############################################################################
# define phony targets for make commands
.PHONY: all all_init clean
# ignore implicit chain and therefore deletion of object files
.SECONDARY: $(SOURCE_FILES_COPY) $(INI_FILES_COPY)
# secondary expansion to allow usage of more complex rules
.SECONDEXPANSION:
# wrape percentage symbol
PERCENT=%
SEMICOLON=;
HASHTAG=\#

all: all_init $(HEX_FILES_EX)


all_init:
	@echo
	@echo "### compiling examples ###"

clean:
	@echo
	@echo "### clean examples ###"
	rm -f *.o *.hex *.obj *.elf *.lss *.lst *.map *.sym
	rm -rf $(PATH_OUT)

call_prerequisite_dot_hex = $(foreach file,$(strip $(filter $(SUFFIX), \
  $(notdir $(wildcard $(notdir $*)/*)))), $(PATH_OUT)$*/$(file))
$(PATH_OUT)%/out.hex: $$(call call_prerequisite_dot_hex) \
  $(SOURCES_LIB) $(HEADERS_LIB)
	@echo
	@echo "## compiling $(notdir $*) for $(*D)"
	$(MAKE) -C $(@D) -f $(MAKEFILE) MCU=$(patsubst %/,%,$(dir $*))

call_prerequisite_other = $(wildcard $(notdir $(*D))/$(notdir $@))
call_recipe_other = \
@if [ "" != "$<" ]$(SEMICOLON) then \
    echo "$(HASHTAG) copy $(notdir $<) for $(patsubst $(PERCENT)/,$(PERCENT),$(dir $(*D)))" $(SEMICOLON)\
    mkdir -p $(@D)$(SEMICOLON) \
    cp $< $@$(SEMICOLON) \
else \
    echo "error - can't find root of file $@"$(SEMICOLON) \
fi
$(PATH_OUT)%.ini: $$(call call_prerequisite_other)
	$(call call_recipe_other)
$(PATH_OUT)%.c: $$(call call_prerequisite_other)
	$(call call_recipe_other)

###############################################################################
