#!/bin/bash

if [ $# != 2 ]; then
    echo "Usage : $0 [inputVideoFileName] [OutputMP3FileName]"
    exit 0
fi
mplayer $1 -ao pcm:file=/dev/fd/1 -really-quiet </dev/null | lame --preset extreme /dev/fd/0 $2 
