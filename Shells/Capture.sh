#!/bin/bash
if [ ! -d ~/Pictures/Capture/`date +%F` ]
then
    mkdir -p ~/Pictures/Capture/`date +%F`
fi
sleep 1
scrot  -s "/home/sigmax6/Pictures/Capture/`date +%F`/%Y-%m-%d-%H-%M-%S-\$wx\$h.png"
