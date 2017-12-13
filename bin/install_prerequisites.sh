#!/usr/bin/env sh

# compiling
sudo apt-get install gcc-avr avr-libc binutils-avr

# documentation
sudo apt-get install doxygen exuberant-ctags

# version control
sudo apt-get install git

# python
sudo apt-get install python python-serial

#sudo apt-get install python-pip
#sudo pip install --upgrade pip
#sudo pip install pyserial

# editor
if [ $# -gt 0 ]; then
    sudo apt-get install kate konsole
fi
