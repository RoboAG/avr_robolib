###############################################################################
#                                                                             #
# src/demos/Makefile                                                          #
# ==================                                                          #
#                                                                             #
# Version: 1.0.3                                                              #
# Date   : 07.02.19                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/RoboAG/robolib                                         #
###############################################################################

# global paths
MAKEFILE  = ../../../../make/Makefile
PATH_TEMP = ../../tmp/
PATH_INC  = ../../include/robolib/
PATH_SRC  = ../robolib/

# specific paths
PATH_OUT  = $(PATH_TEMP)demos/

# suffix of files to be copied
SUFFIX = %.c %.ini $(notdir $*).mcu

###############################################################################
# search for sources (of examples)
DEMOS:= $(patsubst %/,%,$(wildcard */))
SRC_FILES_DM = $(foreach source, $(DEMOS),$(source)/$(source).c)

# create hex-files based on folder-name and mcu-type (single config-files)
HEX_FILES_DM:= $(foreach source, $(DEMOS), $(foreach mcu, \
  $(basename $(notdir $(wildcard $(source)/*.mcu))), \
  $(PATH_OUT)$(source)/$(mcu)/out.hex))
# create hex-files based on folder-name and mcu-type (list of mcu)
HEX_FILES_DM+= $(foreach source, $(DEMOS), \
  $(if $(wildcard $(source)/mcu.txt),$(foreach mcu,$(shell cat \
  $(source)/mcu.txt), $(PATH_OUT)$(source)/$(mcu)/out.hex)))


# search for sources and headers of library (for dependencies)
SOURCES_LIB:= $(wildcard $(PATH_SRC)*/*.c)
HEADERS_LIB:= $(wildcard $(PATH_INC)*.h)

###############################################################################
# define phony targets for make commands
.PHONY: all all_init clean
# ignore implicit chain and therefore deletion of object files
.SECONDARY:
#%.c %.ini %.txt
#$(SOURCE_FILES_COPY) $(INI_FILES_COPY)
# secondary expansion to allow usage of more complex rules
.SECONDEXPANSION:
# wrape percentage symbol
PERCENT=%
SEMICOLON=;
HASHTAG=\#

all: all_init $(HEX_FILES_DM)


all_init:
	@echo
	@echo "### compiling demos ###"

clean:
	@echo
	@echo "### clean demos ###"
	rm -f *.o *.hex *.obj *.elf *.lss *.lst *.map *.sym
	rm -rf $(PATH_OUT)

call_prerequisite_dot_hex = $(foreach file,$(strip $(filter $(SUFFIX), \
  $(notdir $(wildcard $(dir $*)*)))), $(PATH_OUT)$*/$(file))
$(PATH_OUT)%/out.hex: $$(call call_prerequisite_dot_hex) \
  $(SOURCES_LIB) $(HEADERS_LIB)
	@echo
	@echo "## compiling $(*D) for $(notdir $*)"
	$(MAKE) -C $(@D) -f $(MAKEFILE) MCU=$(notdir $*)


call_prerequisite_other = $(wildcard $(dir $(*D))$(notdir $@))
call_recipe_other = \
@if [ "" != "$<" ]$(SEMICOLON) then \
    echo "$(HASHTAG) copy $(notdir $<) for $(notdir $(*D))" $(SEMICOLON)\
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
