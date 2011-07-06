#!/bin/bash

DIR=/home/sigmax6/Documents/
pwd
[ -d $DIR ] && (
cd $DIR
echo "Current DIR is `pwd`"
echo "`ls -l *.h|wc -l`files(*.h)"
pwd
)
pwd
