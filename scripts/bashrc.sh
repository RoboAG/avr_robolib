#!/bin/bash

#***************************[check if already sourced]************************
# 2019 12 01

if [ "$SOURCED_ROBO_ROBOLIB" != "" ]; then

    return
    exit
fi

if [ "$SOURCED_BASH_LAST" == "" ]; then
    export SOURCED_BASH_LAST=1
else
    export SOURCED_BASH_LAST="$(expr "$SOURCED_BASH_LAST" + 1)"
fi

export SOURCED_ROBO_ROBOLIB="$SOURCED_BASH_LAST"


#***************************[paths and files]*********************************
# 2018 11 17

export ROBOLIB_PATH_SCRIPTS="$(cd "$(dirname "${BASH_SOURCE}")" && pwd )/"
export ROBOLIB_PATH="$(cd "${ROBOLIB_PATH_SCRIPTS}../" && pwd )/"

export ROBOLIB_MAKEFILE="${ROBOLIB_PATH}make/Makefile"

export POLOLU_PATH="${ROBOLIB_PATH}../pololu/"


#***************************[repository]**************************************
# 2019 01 29

function robolib_repo_update() {

    cd ${ROBOLIB_PATH}
    make update
}

function robolib_repo_status() {

    cd ${ROBOLIB_PATH}
    make status
}

function robolib_repo_make() {

    cd ${ROBOLIB_PATH}
    make clean
    make warn
}

function robolib_repo_download_additionals() {

    cd ${ROBOLIB_PATH}
    make download_additionals
}

function robolib_repo_install_prerequisites() {

    cd ${ROBOLIB_PATH_SCRIPTS}
    ./install_prerequisites.sh
}


#***************************[local project]***********************************
# 2018 01 28

function robolib_clean_keep_output() {

    make clean_keep_output -f ${ROBOLIB_PATH}make/Makefile
}

function robolib_clean() {

    make clean    -f ${ROBOLIB_MAKEFILE}
}

function robolib_all() {

    make all      -f ${ROBOLIB_MAKEFILE}
}

function robolib_download() {

    make download -f ${ROBOLIB_MAKEFILE}
}

function robolib_avrdude() {

    make avrdude  -f ${ROBOLIB_MAKEFILE}
}

function robolib_data() {

    make data     -f ${ROBOLIB_MAKEFILE}
}


#***************************[pololu]******************************************
# 2018 01 11

function pololu_repo_update() {

    if [ -d "${POLOLU_PATH}" ]; then
        cd ${POLOLU_PATH}
        git pull
    else
        echo "path \"${POLOLU_PATH}\" not found"
    fi
}

function pololu_repo_status() {

    if [ -d "${POLOLU_PATH}" ]; then
        cd ${POLOLU_PATH}
        git status --untracked
    else
        echo "path \"${POLOLU_PATH}\" not found"
    fi
}

function pololu_repo_make() {

    if [ -d "${POLOLU_PATH}" ]; then
        cd ${POLOLU_PATH}
        make clean
        make
    else
        echo "path \"${POLOLU_PATH}\" not found"
    fi
}


#***************************[help]*******************************************
# 2018 01 11

function robolib_help() {

    echo "robolib bash commands"
    echo "====================="
    echo ""
    echo "repository:"
    echo "    robolib_repo_update   pololu_repo_update"
    echo "    robolib_repo_make     pololu_repo_make"
    echo "    robolib_repo_status   pololu_repo_status"
    echo ""
    echo "local project:"
    echo "    robolib_all"
    echo "    robolib_clean"
    echo ""
    echo "    robolib_download"
    echo "    robolib_avrdude"
    echo ""
    echo "    robolib_data"
    echo ""
}
