###############################################################################
#                                                                             #
# bin/Makefile                                                                #
# ============                                                                #
#                                                                             #
# Version: 1.1.1                                                              #
# Date   : 19.02.22                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/RoboAG/avr_robolib                                     #
###############################################################################

NAME_DL=avr_downloader
NAME_XBEE=xbee_config

PATH_REPO=$(NAME_DL)
PATH_BUILD=$(PATH_REPO)/build
FILE_MAKE=$(PATH_REPO)/Makefile

FILE_DL_SRC=$(PATH_REPO)/$(NAME_DL)
FILE_XBEE_SRC=$(PATH_REPO)/$(NAME_XBEE)

URL_REPO=https://github.com/peterweissig/cpp_avr_downloader

###############################################################################
# path and directories

# get current directory
PATH_CUR = $(shell pwd)/

###############################################################################
# define phony targets for make commands
.PHONY: all build clean \
        download_additionals \
        update
# "downloader" and "xbee" are not phony
# (those targets should be created, only if they do not exist)


# build specific commands
all: build

build:
	@echo
	@if [ -f "$(FILE_MAKE)" ]; then \
	    echo "# compile additionals #"; \
	    echo "$(MAKE) -C $(PATH_REPO)"; \
	    $(MAKE) -C $(PATH_REPO); \
	else \
	    echo "# no additional binaries #"; \
	    echo "  use: \$$ make download_additionals"; \
	fi

clean:
ifneq ($(wildcard $(FILE_MAKE)),)
	@echo
	@echo "# clean additionals #"
	$(MAKE) -C $(PATH_REPO) clean
endif


download_additionals: $(FILE_MAKE) build


# git specific commands
update:
	@echo
	@echo "# update additionals #"
	git submodule update

# real targets (not .PHONY)
$(FILE_MAKE):
	@echo "# clone submodule of avr-downloader #"
	git submodule update --init
	@echo

###############################################################################
