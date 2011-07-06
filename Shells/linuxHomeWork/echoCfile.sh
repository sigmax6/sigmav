#!/bin/bash
if [ -f echoCfile.c ]
then
    rm -f echoCfile.c
fi

while read -r line
do
    echo ${line} >> echoCfile.c
done
