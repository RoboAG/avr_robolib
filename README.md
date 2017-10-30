# robolib
Library for using simple avr-controller - especially made for [keplerboard](https://github.com/peterweissig/keplerboard).

## Linux (ubuntu)

Install the following packages:

    sudo apt-get install gcc-avr avr-libc binutils-avr git python python-serial

Additional recommended packages:

    sudo apt-get install kate konsole python-pip doxygen exuberant-ctags
    sudo pip install --upgrade pip
    sudo pip install pyserial

Settings:

    sudo adduser $USER dialout


Download this Repository

    mkdir -p robolib && cd robolib
    git init
    git pull --tags https://github.com/peterweissig/robolib.git

Create Doxygen-Doku

    make doc

Create C-Tags for kate

    make tags

Settings for kate - add build-modul
  * Add new target group (e.g. "robo")

    "atmega64" is just an example - use appropriate controller

    "/dev/ttyUSB0" is just an example - use appropriate interface

  * Add taget "clean"


    make clean   -f /home/xxx/robolib/make/Makefile MCU=atmega64

  * Add target "ini"


    make ini     -f /home/xxx/robolib/make/Makefile MCU=atmega64

  * Add target "build"


    make         -f /home/xxx/robolib/make/Makefile MCU=atmega64

  * Add target "program"


    make program -f /home/xxx/robolib/make/Makefile MCU=atmega64 KATE=1 COMPORT_PROG=/dev/ttyUSB0

  * Add target "terminal"


    make data    -f /home/xxx/robolib/make/Makefile MCU=atmega64 KATE=1 COMPORT_DATA=/dev/ttyUSB0
