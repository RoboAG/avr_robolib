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
# 2022 02 19

function robolib_init_kepler() {

    make init_kepler -f "${ROBOLIB_MAKEFILE}"
}

function robolib_init_3pi() {

    make init_3pi -f "${ROBOLIB_MAKEFILE}"
}


function robolib_all() {

    echo ""
    echo "robolib_all is deprecated - please use **robolib_build** instead"
    echo ""
    robolib_build
}

function robolib_build() {

    # check if optional parameter exists and is not empty
    ARG_MCU=""
    if [ "$1" != "" ]; then
        ARG_MCU="MCU=$1"
    fi

    make build    -f "${ROBOLIB_MAKEFILE}" $ARG_MCU
}


function robolib_clean_keep_output() {

    make clean_keep_output -f "${ROBOLIB_MAKEFILE}"
}

function robolib_clean() {

    make clean    -f "${ROBOLIB_MAKEFILE}"
}


function robolib_avrdude() {

    # check if optional first parameter exists and is not empty
    ARG_COMPORT=""
    if [ "$1" != "" ]; then
        ARG_COMPORT="COMPORT_AVRDUDE=$1"
    fi

    # check if optional second parameter exists and is not empty
    ARG_MCU=""
    if [ "$2" != "" ]; then
        ARG_MCU="MCU=$2"
    fi

    make avrdude  -f "${ROBOLIB_MAKEFILE}" $ARG_COMPORT $ARG_MCU
}

function robolib_download() {

    # check if optional first parameter exists and is not empty
    ARG_COMPORT=""
    if [ "$1" != "" ]; then
        ARG_COMPORT="COMPORT_PROG=$1"
    fi

    # check if optional second parameter exists and is not empty
    ARG_MCU=""
    if [ "$2" != "" ]; then
        ARG_MCU="MCU=$2"
    fi

    make download -f "${ROBOLIB_MAKEFILE}" $ARG_COMPORT $ARG_MCU
}


function robolib_data() {

    # check if optional first parameter exists and is not empty
    ARG_COMPORT=""
    if [ "$1" != "" ]; then
        ARG_COMPORT="COMPORT_DATA=$1"
    fi

    # check if optional second parameter exists and is not empty
    ARG_BAUDRATE=""
    if [ "$2" != "" ]; then
        ARG_BAUDRATE="BAUDRATE_DATA=$2"
    fi

    # run Makefile
    make data     -f "${ROBOLIB_MAKEFILE}" $ARG_COMPORT $ARG_BAUDRATE
}

function robolib_xbee() {

    # check if optional parameter exists and is not empty
    ARG_COMPORT=""
    if [ "$1" != "" ]; then
        ARG_COMPORT="COMPORT_DATA=$1"
    fi

    make xbee_check -f "${ROBOLIB_MAKEFILE}" $ARG_COMPORT
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


#***************************[backwards compatibility]*************************
# 2022 02 19

temp_path="${ROBOLIB_PATH}bin/avr_downloader/"
if [ -e "${temp_path}Makefile" ]; then

    (
        cd "$temp_path"
        if git remote -v | grep --silent cpp_main; then
            echo "warning: Changed structure of avr-downloader on 18.02.2022!"
            echo "  You should remove the files accordingly:"
            echo "    $ rm -rf \"${temp_path}*\""
            temp_path="${ROBOLIB_PATH}bin/xbee_config/"
            if [ -d "${temp_path}" ]; then
                echo "    $ rm -rf \"${temp_path}\""
            fi
            files="downloader xbee"
            temp_path="${ROBOLIB_PATH}bin/"
            for file in $files; do
                if [ -f "${temp_path}${file}" ]; then
                    echo "    $ rm \"${temp_path}${file}\""
                fi
            done
            echo "  Download submodule:"
            echo "    $ robolib_repo_download_additionals"
        fi
    )
fi


#***************************[help]*******************************************
# 2022 02 19

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
    echo "    robolib_init_kepler"
    echo "    robolib_init_3pi"
    echo ""
    echo "    robolib_build [mcu]"
    echo "    robolib_clean"
    echo ""
    echo "    robolib_download [comport [mcu]]"
    echo "    robolib_avrdude [comport [mcu]]"
    echo ""
    echo "    robolib_data [comport [baudrate]]"
    echo "    robolib_xbee [comport]"
    echo ""
}
