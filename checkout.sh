#!/bin/sh

###############################################################################
#                                                                             #
# checkout.sh                                                                 #
# ===========                                                                 #
#                                                                             #
# Version: 1.1.0                                                              #
# Date   : 25.10.15                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/peterweissig/robolib/                                  #
###############################################################################

NAME_GIT_THIS="robolib"

###############################################################################
URL_GIT_BASE="https://github.com/peterweissig/"
URL_GIT_THIS="${URL_GIT_BASE}${NAME_GIT_THIS}.git"

NAME_CHECKOUT_SCRIPT="checkout.sh"

###############################################################################
echo "The project"
echo "  \"${NAME_GIT_THIS}\""
echo "will be checked out completely."
echo ""


echo ""
echo "### renaming this script"
mv "${NAME_CHECKOUT_SCRIPT}" "_${NAME_CHECKOUT_SCRIPT}"

echo ""
echo "### checking out the project"
if [ -d ".git" ]; then
    echo "This folder already is a git-repository!"
    return
fi
git init
git pull "${URL_GIT_THIS}"

echo ""
echo "### updating and downloading additonal files"
if [ ! -f "Makefile" ]; then
    echo "Error - no Makefile"
    return
fi
make update

echo ""
echo "### deleting this script"
rm "_${NAME_CHECKOUT_SCRIPT}"
