###############################################################################
#                                                                             #
# Makefile                                                                    #
# ========                                                                    #
#                                                                             #
# Version: 1.2.0                                                              #
# Date   : 06.11.17                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/peterweissig/robolib                                   #
###############################################################################

NAME_GIT_THIS=robolib

SUBDIRS = bin tags doc

###############################################################################
# define phony targets for make commands
.PHONY: all all_init  $(SUBDIRS)  additionals  clean \
        update status push pull

all: all_init $(SUBDIRS)

all_init:
	@echo "### building $(NAME_GIT_THIS) ###"


$(SUBDIRS):
	$(MAKE) -C $@


additionals:
	$(MAKE) -C bin/ additionals


clean:
	@echo
	@echo "### clean $(NAME_GIT_THIS) ###"
	@for dir in $(SUBDIRS); do \
	    $(MAKE) -C $$dir clean; \
	done


update: pull
	$(MAKE) -C bin/ update

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
