#***************************[paths and files]*********************************
# 2018 01 11

export ROBOLIB_PATH_SCRIPTS="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd )/"
export ROBOLIB_PATH="$(cd "${ROBOLIB_PATH_SCRIPTS}../" \
  && pwd )/"

export ROBOLIB_MAKEFILE="${ROBOLIB_PATH}make/Makefile"


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


#***************************[local project]***********************************
# 2018 01 11

function robolib_clean() {

    make clean -f ${ROBOLIB_PATH}make/Makefile
}

function robolib_all() {

    make all -f ${ROBOLIB_PATH}make/Makefile
}

function robolib_dowload() {

    make download -f ${ROBOLIB_PATH}make/Makefile
}

function robolib_avrdude() {

    make avrdude -f ${ROBOLIB_PATH}make/Makefile
}

function robolib_data() {

    make data -f ${ROBOLIB_PATH}make/Makefile
}

