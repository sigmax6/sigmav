#!/bin/bash

list_alldir(){
    for file in $1/*
    do
        if [ -d $file ]; then
            echo $file          #Do the stuff here
            list_alldir $file   #Recursion    
        fi
    done
}

if [ $# -gt 0 ]
then
    list_alldir "$1"
else
    list_alldir "."
fi
