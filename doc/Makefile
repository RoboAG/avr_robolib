###############################################################################
#                                                                             #
# doc/Makefile                                                                #
# ============                                                                #
#                                                                             #
# Version: 1.0.0                                                              #
# Date   : 06.11.17                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/peterweissig/robolib                                   #
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
