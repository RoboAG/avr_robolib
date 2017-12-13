###############################################################################
#                                                                             #
# src/examples/Makefile                                                       #
# =====================                                                       #
#                                                                             #
# Version: 1.2.1                                                              #
# Date   : 13.12.17                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/peterweissig/robolib                                   #
###############################################################################

# global paths
MAKEFILE  = ../../../../make/Makefile
PATH_TEMP = ../../tmp/
PATH_INC  = ../../include/robolib/
PATH_SRC  = ../robolib/

# specific paths
PATH_OUT  = $(PATH_TEMP)examples/

# suffix of files to be copied
SUFFIX = %.c %.ini $(*D).mcu

###############################################################################
# search for sources (of examples)
EXAMPLES  = $(patsubst %/,%,$(wildcard */))
SRC_FILES_EX = $(foreach source, $(EXAMPLES),$(source)/$(source).c)

# create hex-files based on folder-name and mcu-type (single config-files)
HEX_FILES_EX = $(foreach source, $(EXAMPLES), $(foreach mcu, \
  $(basename $(notdir $(wildcard $(source)/*.mcu))), \
  $(PATH_OUT)$(mcu)/$(source)/out.hex))

# create hex-files based on folder-name and mcu-type (list of mcu)
HEX_FILES_EX+= $(foreach source, $(EXAMPLES), \
  $(if $(wildcard $(source)/mcu.txt),$(foreach mcu,$(shell cat \
  $(source)/mcu.txt), $(PATH_OUT)$(mcu)/$(source)/out.hex)))


# search for sources and headers of library (for dependencies)
SOURCES_LIB = $(wildcard $(PATH_SRC)*/*.c)
HEADERS_LIB = $(wildcard $(PATH_INC)*.h)

###############################################################################
# define phony targets for make commands
.PHONY: all all_init clean
# ignore implicit chain and therefore deletion of object files
.SECONDARY:
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
	$(MAKE) -C $(@D) -f $(MAKEFILE) \
	  $(shell if [ -e "$(PATH_OUT)$*/$(*D).mcu" ]; then \
	  cat $(PATH_OUT)$*/$(*D).mcu; \
	  else echo "MCU=$(*D)"; fi)


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
$(PATH_OUT)%.mcu: $$(call call_prerequisite_other)
	$(call call_recipe_other)

###############################################################################
