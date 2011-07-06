#!/bin/bash

for i in "$@"
do
    find "$i" -name "*.c" -print -exec rm -rf {} \;
    [ "$i/*" = $i/* ] && {
    rmdir "$i"
    echo "Delete dir $i"
}
   # echo "$i/*"
   # echo $i/*
done
