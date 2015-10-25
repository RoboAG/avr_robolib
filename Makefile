###############################################################################
#                                                                             #
# Makefile                                                                    #
# ========                                                                    #
#                                                                             #
# Version: 1.0.3                                                              #
# Date   : 25.10.15                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/peterweissig/robolib                                   #
###############################################################################

NAME_GIT_THIS=robolib

URL_GIT_BASE=https://github.com/peterweissig/
URL_GIT_THIS=$(URL_GIT_BASE)$(NAME_GIT_THIS).git

###############################################################################
NAME_DOWNLOADER=downloader
NAME_DOWNLOADER64=$(NAME_DOWNLOADER)_64
NAME_DOWNLOADER32=$(NAME_DOWNLOADER)_32

PATH_DOWNLOADER=bin/additionals/

URL_GIT_DOWNLOADER=$(URL_GIT_BASE)cpp_avr_downloader/raw/master/bin/
URL_GIT_DOWNLOADER64=$(URL_GIT_DOWNLOADER)$(NAME_DOWNLOADER64)
URL_GIT_DOWNLOADER32=$(URL_GIT_DOWNLOADER)$(NAME_DOWNLOADER32)

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
.PHONY: all doc tags clean update status push

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

clean:
	@echo
	@echo "### clean $(NAME_GIT_THIS) ###"
	rm -rfv $(PATH_MAKE)doc/html/*
	rm -fv $(PATH_MAKE)examples/Makefile
	rm -rfv $(PATH_MAKE)tags/*

update:
	@echo ""
	@echo "### update $(NAME_GIT_THIS) ###"
	git pull "$(URL_GIT_THIS)"

	-rm $(PATH_DOWNLOADER)$(NAME_DOWNLOADER64)
	wget -nv --directory-prefix=$(PATH_DOWNLOADER) $(URL_GIT_DOWNLOADER64)

#	-rm $(PATH_DOWNLOADER)$(NAME_DOWNLOADER32)
#	wget -nv --directory-prefix=$(PATH_DOWNLOADER) $(URL_GIT_DOWNLOADER32)

ifneq ($(strip $(shell uname -m | grep -o 64)),)
	@echo "using 64-bit version of downloader"
	@cp $(PATH_DOWNLOADER)$(NAME_DOWNLOADER64)                            \
          $(PATH_DOWNLOADER)../$(NAME_DOWNLOADER)
endif


status:
	@echo ""
	@echo "### status of $(NAME_GIT_THIS) ###"
	@git status --untracked-files

push:
	@echo ""
	@echo "### pushing of $(NAME_GIT_THIS) ###"
	git push "$(URL_GIT_THIS)"

###############################################################################
