###############################################################################
#                                                                             #
# bin/Makefile                                                                #
# ============                                                                #
#                                                                             #
# Version: 1.0.2                                                              #
# Date   : 11.01.18                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/peterweissig/robolib                                   #
###############################################################################

NAME_DL=avr_downloader
NAME_XBEE=xbee_config
PATH_DL=$(NAME_DL)/
PATH_XBEE=$(NAME_XBEE)/

DEST_DL=downloader
DEST_XBEE=xbee
PATH_DL_BIN=$(NAME_DL)/build/$(NAME_DL)/$(NAME_DL)
PATH_XBEE_BIN=$(NAME_XBEE)/build/$(NAME_XBEE)/$(NAME_XBEE)

URL_GIT_BASE=https://github.com/peterweissig/
URL_CO_DL=$(URL_GIT_BASE)cpp_$(NAME_DL)/raw/master/checkout.sh
URL_CO_XBEE=$(URL_GIT_BASE)cpp_$(NAME_XBEE)/raw/master/checkout.sh

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
ifneq ($(firstword $(wildcard $(PATH_DL) $(PATH_XBEE))),)
	@echo "# compile additionals #"
	-$(MAKE) -C $(PATH_DL)
	-$(MAKE) -C $(PATH_XBEE)
else
	@echo "# no additional binaries #"
	@echo "  use: \$$ make download_additionals"
endif

clean:
ifneq ($(firstword $(wildcard $(PATH_DL) $(PATH_XBEE))),)
	@echo
	@echo "# clean additionals #"
	-$(MAKE) -C $(PATH_DL) clean
	-$(MAKE) -C $(PATH_XBEE) clean
endif


download_additionals: $(DEST_DL) $(DEST_XBEE)


# git specific commands
update:
	@echo
ifneq ($(firstword $(wildcard $(PATH_DL) $(PATH_XBEE))),)
	@echo "# update additionals #"
	-$(MAKE) -C $(PATH_DL) update
	-$(MAKE) -C $(PATH_XBEE) update
else
	@echo "# no additional binaries #"
	@echo "  use: \$$ make additionals"
endif

# real targets (not .PHONY)
$(DEST_DL): $(PATH_DL_BIN)
	@echo "# create softlink for downloader #"
	-ln -s $(PATH_DL_BIN) $(DEST_DL)

$(PATH_DL_BIN): $(PATH_DL)
	@echo "# compile avr-downloader #"
	$(MAKE) -C $(PATH_DL)

$(PATH_DL):
	@echo "# download avr-downloader #"
	wget -nv $(URL_CO_DL)
	bash ./checkout.sh
	@echo

$(DEST_XBEE): $(PATH_XBEE_BIN)
	@echo "# create softlink for xbee #"
	-ln -s $(PATH_XBEE_BIN) $(DEST_XBEE)

$(PATH_XBEE_BIN): $(PATH_XBEE)
	@echo "# compile xbee-tool #"
	$(MAKE) -C $(PATH_XBEE)

$(PATH_XBEE):
	@echo "# download xbee-tool #"
	wget -nv $(URL_CO_XBEE)
	bash ./checkout.sh
	@echo

###############################################################################
