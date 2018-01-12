###############################################################################
#                                                                             #
# Makefile                                                                    #
# ========                                                                    #
#                                                                             #
# Version: 1.2.4                                                              #
# Date   : 13.12.17                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/peterweissig/robolib                                   #
###############################################################################

NAME_GIT_THIS=robolib

SUBDIRS = src tags doc bin

###############################################################################
# define phony targets for make commands
.PHONY: all all_init warn  $(SUBDIRS) clean \
        install_prerequisites download_additionals \
        update status push pull


# build specific commands
all: all_init $(SUBDIRS)

all_init:
	@echo
	@echo "### building $(NAME_GIT_THIS) ###"

warn:
	@echo
	@echo "###### reducing output to warnings ######"
	$(MAKE) | grep -A 3 -B 3 -i warn; dummy=$?

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	@echo
	@echo "### clean $(NAME_GIT_THIS) ###"
	@for dir in $(SUBDIRS); do \
	    $(MAKE) -C $$dir clean; \
	done


# install and download commands
install_prerequisites:
	@cd scripts && ./install_prerequisites.sh

download_additionals:
	$(MAKE) -C bin/ download_additionals


# git specific commands
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
