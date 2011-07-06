#!/bin/bash


list_alldir(){
    for file in $1/*
    do
        if [ -d $file ]; then
           # echo $file          #Do the stuff here
            find $file  -maxdepth 1 -type f -exec cp {} $2/ \;
           # find $file ! -name "." -type d -prune -o -type f
            list_alldir $file   #Recursion    
        fi
    done
}

if [ $# -eq 2 ]
then
    list_alldir "$1" "$2"
elif [ $# -eq 1 ]
then
    list_alldir "." "$1"
else
    echo "Usage : $0 <src dir> [dist dir]"
fi
