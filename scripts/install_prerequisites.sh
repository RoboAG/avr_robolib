#***************************[system]******************************************
# 2018 01 11

sudo apt-get update
if [ $? -ne 0 ]; then return -1; fi


#***************************[compiler]****************************************
# 2018 01 11

sudo apt-get install -y gcc-avr avr-libc binutils-avr


#***************************[tools]*******************************************
# 2018 01 11

sudo apt-get install -y git
sudo apt-get install -y make


#***************************[downloader]**************************************
# 2018 01 11

sudo apt-get install -y binutils avrdude
sudo apt-get install -y g++ cmake


#***************************[data]********************************************
# 2018 01 11

sudo apt-get install -y python python-serial

#sudo apt-get install python-pip
#sudo pip install --upgrade pip
#sudo pip install pyserial

#***************************[documentation]***********************************
# 2018 01 18

sudo apt-get install -y doxygen
    sudo apt-get install -y exuberant-ctags


#***************************[additionals]*************************************
# 2018 01 18

if [ $# -gt 0 ]; then
    sudo apt-get install -y kate
    sudo apt-get install -y konsole
fi


#***************************[done]********************************************

echo "all done :-)"
