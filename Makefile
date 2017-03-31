###############################################################################
#                                                                             #
# Makefile                                                                    #
# ========                                                                    #
#                                                                             #
# Version: 1.1.1                                                              #
# Date   : 31.03.17                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/peterweissig/robolib                                   #
###############################################################################

NAME_GIT_THIS=robolib

URL_GIT_BASE=https://github.com/peterweissig/
URL_GIT_THIS=$(URL_GIT_BASE)$(NAME_GIT_THIS).git

###############################################################################
PATH_ADDITIONALS=bin/additionals/
PATH_DOWNLOADER=bin/downloader
PATH_XBEE=bin/xbee

PATH_DOWNLOADER_BIN=$(PATH_ADDITIONALS)/build/avr_downloader/avr_downloader
PATH_XBEE_BIN=$(PATH_ADDITIONALS)/build/xbee_config/xbee_config

URL_CO_ADDITIONALS=$(URL_GIT_BASE)cpp_avr_downloader/raw/master/checkout.sh

###############################################################################
# path and directories

# get current directory
PATH_CUR = $(shell pwd)/

# get current directory of this makefile
PATH_MAKE = $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))/

###############################################################################
# header-files and source-files

# list of files for "ctags"
HEADER_FILES     = $(wildcard $(PATH_MAKE)system/*.h)
ifneq ($(strip $(HEADER_FILES)),)
  SOURCE_FILES = $(foreach header,$(basename $(notdir \
    $(HEADER_FILES)))    , $(PATH_MAKE)source/$(header)/$(header).c)
endif
NEW_HEADER_FILES = $(wildcard $(PATH_MAKE)_system/*.h)
ifneq ($(strip $(NEW_HEADER_FILES)),)
  NEW_SOURCE_FILES = $(foreach header,$(basename $(notdir \
    $(NEW_HEADER_FILES))), $(PATH_MAKE)_source/$(header)/$(header).c)
endif

###############################################################################
# define phony targets for make commands
.PHONY: all doc tags additionals clean update status push pull

all: doc tags

doc:
	@echo
	@echo "### create documentation ###"
	@cp $(PATH_MAKE)make/Makefile $(PATH_MAKE)examples/Makefile
	@cd $(PATH_MAKE)doc/ && doxygen

tags:
	@echo
	@echo "### create ctags ###"
	mkdir -p $(PATH_MAKE)tags
	@cd $(PATH_MAKE)tags && ctags --c++-types=+px --extra=+q              \
          --excmd=pattern $(SOURCE_FILES) $(HEADER_FILES) $(NEW_SOURCE_FILES) \
          $(NEW_HEADER_FILES)

additionals:
	@echo
	@echo "### create additonal binarys ###"
ifeq ($(firstword $(wildcard $(PATH_MAKE)$(PATH_ADDITIONALS))),)
	@echo "* downloading source-files *"
	@mkdir -p $(PATH_MAKE)$(PATH_ADDITIONALS)
	wget -nv --directory-prefix=$(PATH_MAKE)$(PATH_ADDITIONALS) \
	  $(URL_CO_ADDITIONALS)
	cd $(PATH_MAKE)$(PATH_ADDITIONALS) && bash ./checkout.sh
	@echo
endif
	@echo "* compiling *"
	cd $(PATH_MAKE)$(PATH_ADDITIONALS) && make
ifeq ($(firstword $(wildcard $(PATH_MAKE)$(PATH_DOWNLOADER))),)
	@echo
	@echo "* linking *"
	ln -s $(PATH_MAKE)$(PATH_DOWNLOADER_BIN) $(PATH_MAKE)$(PATH_DOWNLOADER)
endif

clean:
	@echo
	@echo "### clean $(NAME_GIT_THIS) ###"
	rm -rfv $(PATH_MAKE)doc/html/*
	rm -fv $(PATH_MAKE)examples/Makefile
	rm -rfv $(PATH_MAKE)tags/*

update: pull

status:
	@echo ""
	@echo "### status of $(NAME_GIT_THIS) ###"
	@git status --untracked-files

push:
	@echo ""
	@echo "### pushing of $(NAME_GIT_THIS) ###"
	git push

pull:
	@echo ""
	@echo "### update $(NAME_GIT_THIS) ###"
	git pull


###############################################################################
