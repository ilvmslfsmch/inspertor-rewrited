#! /usr/bin/bash

sudo add-apt-repository universe -y && sudo apt-get update
sudo apt-get upgrade
sudo apt-get install tmux unzip python3 python3-pip python3-future libfuse2 linux-firmware ./KasperskyOS-Community-Edition-Qemu-1.4.0.102_ru
sudo pip3 install pyserial mavproxy
pip install --target mavproxy/ mavproxy
sudo chmod -R 777 mavproxy
