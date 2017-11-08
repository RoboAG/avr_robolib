###############################################################################
#                                                                             #
# src/Makefile                                                                #
# ============                                                                #
#                                                                             #
# Version: 1.0.0                                                              #
# Date   : 06.11.17                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/peterweissig/robolib                                   #
###############################################################################

#SUBDIRS = robolib examples
SUBDIRS = examples

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
