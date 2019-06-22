# avr_robolib
Library for using simple avr-controller - especially made for [keplerboard](https://github.com/RoboAG/eagle_keplerboard).

[![Build Status](https://travis-ci.org/RoboAG/avr_robolib.svg?branch=master)](https://travis-ci.org/RoboAG/avr_robolib)



## Documentation
Any change to this repository will trigger a new build of the [current documentation](https://roboag.github.io/doc_robolib/).


## Regular usage
### Compiling your code
Switch to your source code folder:

    cd path/to/your/source/code/

Call needed make-command

    # build
    make -f <path to robolib>/make/Makefile

    # clear directory from build
    make clean -f <path to robolib>/make/Makefile

    # download via downloader (needs bootloader)
    make download -f <path to robolib>/make/Makefile

    # download via avrdude
    make avrdude -f <path to robolib>/make/Makefile

    # open simple python terminal
    make data -f <path to robolib>/make/Makefile

    # ...


### Update robolib
Switch to library folder:

    cd robolib

Update library and binaries:

    make update

Build library:

    make


### Setup for linux (Ubuntu)
Install the following packages:

    sudo apt-get install git make

Download this Repository:

    git clone https://github.com/RoboAG/avr_robolib.git robolib

Switch to library folder:

    cd robolib

Install packages ([see also here](scripts/install_prerequisites.sh)) :

    make install_prerequisites


Download binaries:

    make download_additionals

Build library:

    make

Settings:

    sudo adduser $USER dialout



## Using Bash-Scripts
For these commands to work, you need to source the robolib - see also setup. Afterwards you don't need to switch directory to the library for any command.

### Compiling your code
Switch to your source code folder:

    cd path/to/your/source/code/

Call needed make-command

    # build
    robolib_all

    # clear directory from build
    robolib_clean

    # download via downloader (needs bootloader)
    robolib_download

    # download via avrdude
    robolib_avrdude

    # open simple python terminal
    robolib_data

    # ...


### Update

Update library and binaries:

    robolib_repo_update

Build library:

    robolib_repo_make


## Setup for linux (Ubuntu)
Install the following packages:

    sudo apt-get install git make

Download this Repository:

    git clone https://github.com/RoboAG/avr_robolib.git robolib

Switch to library folder:

    cd robolib

Automatically source robolib:

    . scripts/setup_bashrc.sh
    # answer question with "yes"

Close terminal and open a new one

Install packages ([see also here](scripts/install_prerequisites.sh)) :

    robolib_repo_install_prerequisites

Download binaries:

    robolib_repo_download_additionals

Build library:

    robolib_repo_make

Settings:

    sudo adduser $USER dialout
