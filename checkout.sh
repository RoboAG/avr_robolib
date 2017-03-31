#!/bin/sh

###############################################################################
#                                                                             #
# checkout.sh                                                                 #
# ===========                                                                 #
#                                                                             #
# Version: 1.2.0                                                              #
# Date   : 31.03.17                                                           #
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
echo "### checking out the project"
if [ -d "${NAME_GIT_THIS}" ]; then
    echo "This project already exists!"
    return
fi
git clone "${URL_GIT_THIS}" "${NAME_GIT_THIS}"

echo ""
echo "### downloading additonal files"
echo "Do you want to download additional binarys ?"
echo "(This can also be done later be invocing \"make additionals\")"
read answer;
if [ "$answer" == "yes" ] || [ "$answer" == "Yes" ] || \
   [ "$answer" == "y" ] || [ "$answer" == "Y" ] || \
   [ "$answer" == "YES" ]; then
    make -f "${NAME_GIT_THIS}/Makefile" additionals
fi

if [ $? -eq 0 ]; then
    echo ""
    echo "### deleting this script"
    rm "${NAME_CHECKOUT_SCRIPT}"
fi
