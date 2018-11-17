# avr_robolib
Library for using simple avr-controller - especially made for [keplerboard](https://github.com/RoboAG/eagle_keplerboard).

[![Build Status](https://travis-ci.org/RoboAG/avr_robolib.svg?branch=master)](https://travis-ci.org/RoboAG/avr_robolib)

## Test library and build documentaion

    cd robolib
    make


## Setup for linux (Ubuntu)

Install the following packages:

    sudo apt-get install git make


Download this Repository

    git clone https://github.com/RoboAG/avr_robolib.git robolib


Install packages ([see also here](scripts/install_prerequisites.sh)) :

    cd robolib
    make install_prerequisites


Download binaries:

    #cd robolib
    make download_additionals


Settings:

    sudo adduser $USER dialout

