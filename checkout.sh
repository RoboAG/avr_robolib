#!/bin/sh

###############################################################################
#                                                                             #
# checkout.sh                                                                 #
# ===========                                                                 #
#                                                                             #
# Version: 1.2.3                                                              #
# Date   : 13.03.18                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/RoboAG/avr_robolib/                                    #
###############################################################################

NAME_THIS="robolib"

###############################################################################
PATH_THIS="${NAME_THIS}/"

NAME_GIT_THIS="avr_${NAME_THIS}"

URL_GIT_BASE="https://github.com/RoboAG/"
URL_GIT_THIS="${URL_GIT_BASE}${NAME_GIT_THIS}.git"

NAME_CHECKOUT_SCRIPT="checkout.sh"

###############################################################################
echo "The project"
echo "  \"${NAME_THIS}\""
echo "will be checked out completely."
echo ""


echo ""
echo "### checking out the project"
if [ -d "${PATH_THIS}" ]; then
    echo "This project already exists!"
    return
fi
git clone "${URL_GIT_THIS}" "${PATH_THIS}"


echo ""
echo "### automatically sourcing this project"
./${PATH_THIS}scripts/setup_bashrc.sh


echo ""
echo "### downloading additonal files"
echo "Do you want to download additional binarys ?"
echo "(This can also be done later be invocing \"make additionals\")"
read answer;
if [ "$answer" == "yes" ] || [ "$answer" == "Yes" ] || \
   [ "$answer" == "y" ] || [ "$answer" == "Y" ] || \
   [ "$answer" == "YES" ]; then
    make -f "${PATH_THIS}/Makefile" additionals
fi


if [ $? -ne 0 ]; then
    echo "### There have been errors! ###"
    return -1;
else
    echo ""
    echo "### deleting this script"
    rm "${NAME_CHECKOUT_SCRIPT}"

    echo "all done :-)"
fi
