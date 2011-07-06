#!/bin/bash

echo "\$* is :$* "
echo "\$@ is :$@ "

#count=0
echo ${count:-0}
echo ${count2:=10}
echo ${count2:?"undefined!"}
count=1
echo ${count:+1}
