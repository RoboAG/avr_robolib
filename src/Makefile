###############################################################################
#                                                                             #
# src/Makefile                                                                #
# ============                                                                #
#                                                                             #
# Version: 1.0.2                                                              #
# Date   : 07.02.19                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/RoboAG/robolib                                         #
###############################################################################

#SUBDIRS = robolib examples demos
SUBDIRS = examples demos

###############################################################################
# define phony targets for make commands
.PHONY: all all_init  $(SUBDIRS)  clean

all: all_init $(SUBDIRS)

all_init:
	@echo "# building sources #"


$(SUBDIRS):
	$(MAKE) -C $@


clean:
	@echo
	@echo "# clean sources #"
	@for dir in $(SUBDIRS); do \
	    $(MAKE) -C $$dir clean; \
	done

###############################################################################
