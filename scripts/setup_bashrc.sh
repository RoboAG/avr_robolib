#!/bin/bash

echo ""
echo "setup_bashrc.sh script was called."
echo "The following library will be sourced within your bashrc."
echo "    robolib"
echo "Do you wish to continue ? (No/yes)"
if [ "$1" != "-y" ] && [ "$1" != "--yes" ]; then
    read answer
else
    echo "<auto answer \"yes\">"
    answer="yes"
fi
if [ "$answer" != "y" ] && [ "$answer" != "Y" ] && \
  [ "$answer" != "yes" ]; then

    echo "Your ~./bashrc was NOT changed."
else

    # get local directory
    SCRIPTDIR="$( cd "$( dirname "${BASH_SOURCE}" )" && pwd )/"

    BASHRC_SOURCE=". ${SCRIPTDIR}bashrc.sh"
    if grep -Fq "${BASHRC_SOURCE}" ~/.bashrc; then

        echo "robolib already sourced within bashrc. This is good!"
    else

        echo "Adding robolib to your bashrc."

        echo ""                                        >> ~/.bashrc
        echo "# $(date +"%Y_%m_%d") sourcing robolib:" >> ~/.bashrc
        echo "$BASHRC_SOURCE"                          >> ~/.bashrc
    fi
fi
