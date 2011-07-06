#!/bin/bash
eog ../BmpParse/lenaOut.bmp &
./simulation.x &
sleep 15 
eog lenaResOut.bmp &
pkill simulation.x
pkill xterm
