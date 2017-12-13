# robolib
Library for using simple avr-controller - especially made for [keplerboard](https://github.com/peterweissig/eagle_keplerboard).

## Setup for linux (Ubuntu)

Install the following packages:

    sudo apt-get install git


Download this Repository

    git clone https://github.com/peterweissig/robolib.git


Install packages ([see also here](bin/install_prerequisites.sh)) :

    cd robolib
    make install_prerequisites


Download binaries:

    #cd robolib
    make download_additionals


Settings:

    sudo adduser $USER dialout


## Test library and build documentaion

    #cd robolib
    make
