#!/bin/bash

read -r -p "Installing Dependencies...? [y/N] " response
if [[ $response =~ ^([yY][eE][sS]|[yY])$ ]]
then
    sudo apt-get install g++ build-essential libtool autoconf pkg-config libboost-system-dev libboost-filesystem-dev libboost-test-dev libboost-thread-dev libboost-program-options-dev cmake libjsoncpp-dev libsodium-dev git
    if [[ $? > 0 ]]
    then
        echo "Installation failed, exiting."
        exit
    fi
fi

echo "Cloning necessary repositories..."
mkdir -p repos && cd repos

echo "Compiling zeromq4-1"
git clone https://github.com/zeromq/zeromq4-1.git
cd zeromq4-1/
./autogen.sh
./configure && make
sudo make install
sudo ldconfig
cd ..

echo "Compiling zmqpp"
git clone https://github.com/zeromq/zmqpp.git
cd zmqpp/
make
sudo make install
cd ../..


echo "Compiling flocksy"
mkdir -p build && cd build
cmake ..
make
cd ../bin

echo "Creating folders in /tmp/"
mkdir -p /tmp/testdir1
mkdir -p /tmp/testdir2
mkdir -p /tmp/testdir3

echo "All set!"
echo "To use the example configurations, type in separate shells: "
echo "./flocksy -c ../test/configX --keystore ../test/flocksy_keystore --privatekey ../test/flocksy_privatekeys"
echo "(with X in {1,2,3})."
