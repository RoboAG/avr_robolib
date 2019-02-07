###############################################################################
#                                                                             #
# doc/Makefile                                                                #
# ============                                                                #
#                                                                             #
# Version: 1.0.1                                                              #
# Date   : 07.02.19                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/RoboAG/robolib                                         #
###############################################################################

# define phony targets for make commands
.PHONY: all doc doc_init clean

all: doc

doc: doc_init
	@doxygen

doc_init:
	@echo
	@echo "# create documentation #"

clean:
	@echo
	@echo "# clean documentation #"
	rm -rf html/

###############################################################################
