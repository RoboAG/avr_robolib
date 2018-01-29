#!/bin/bash

echo ""
echo "setup_bashrc.sh script was called."
echo "The following library will be sourced within your bashrc."
echo "    robolib"
echo "Do you want to continue? (y/N)"
read -s -n 1 ANS; echo ""
if [ "$ANS" == "y" ]; then

    # get local directory
    SCRIPTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/"

    BASHRC_SOURCE=". ${SCRIPTDIR}bashrc.sh"
    if grep -Fq "${BASHRC_SOURCE}" ~/.bashrc; then

        echo "robolib already sourced within bashrc. This is good!"
    else

        echo "Adding robolib to your bashrc."

        echo ""                                        >> ~/.bashrc
        echo "# $(date +"%Y_%m_%d") sourcing robolib:" >> ~/.bashrc
        echo "$BASHRC_SOURCE"                          >> ~/.bashrc
    fi
else

    echo "Your ~./bashrc was NOT changed."
fi
