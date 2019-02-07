###############################################################################
#                                                                             #
# tags/Makefile                                                               #
# =============                                                               #
#                                                                             #
# Version: 1.0.1                                                              #
# Date   : 07.02.19                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/RoboAG/robolib                                         #
###############################################################################

# find all header and source files for indexing
SOURCE=$(wildcard ../include/robolib/*.h) \
  $(wildcard ../src/robolib/*/*.c)

###############################################################################
# define phony targets for make commands
.PHONY: all tags tags_init clean

all: tags

tags: tags_init
	ctags --c++-types=+px --extra=+q --excmd=pattern --totals=yes \
	  $(SOURCE)

tags_init:
	@echo
	@echo "# create c-tags #"

clean:
	@echo
	@echo "# clean c-tags #"
	rm -f $$(ls | grep -v Makefile)

###############################################################################
