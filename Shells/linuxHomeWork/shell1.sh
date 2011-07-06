#!/bin/bash

#if [ -e ]
#touch temp.c
cat > temp.c
echo "The bytes in the file tmp is : `wc -c tmp`"
ls -l tmp
gcc -o temp temp.c
./temp
