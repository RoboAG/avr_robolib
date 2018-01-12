#***************************[paths and files]*********************************
# 2018 01 11

export ROBOLIB_PATH_SCRIPTS="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd )/"
export ROBOLIB_PATH="$(cd "${ROBOLIB_PATH_SCRIPTS}../" && pwd )/"

export ROBOLIB_MAKEFILE="${ROBOLIB_PATH}make/Makefile"

export POLOLU_PATH="${ROBOLIB_PATH}../pololu/"


#***************************[repository]**************************************
# 2018 01 11

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

function robolib_repo_additionals() {

    cd ${ROBOLIB_PATH}
    make download_additionals
}


#***************************[local project]***********************************
# 2018 01 11

function robolib_clean() {

    make clean -f ${ROBOLIB_PATH}make/Makefile
}

function robolib_all() {

    make all -f ${ROBOLIB_PATH}make/Makefile
}

function robolib_download() {

    make download -f ${ROBOLIB_PATH}make/Makefile
}

function robolib_avrdude() {

    make avrdude -f ${ROBOLIB_PATH}make/Makefile
}

function robolib_data() {

    make data -f ${ROBOLIB_PATH}make/Makefile
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


