#!/bin/bash
make 
make daemon 

sudo rmmod usbkey
sudo dmesg -C 
sudo -v 

sudo dmesg -w | grep --line-buffered usbkey &
./usbkeyd &

sudo insmod usbkey.ko vid=0x13fe pid=0x6300 serial="07192164148CE881"


