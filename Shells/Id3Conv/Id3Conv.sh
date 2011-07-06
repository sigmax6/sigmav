#/bin/bash

ID3CONV=/home/sigmax6/Documents/Workspace/sigmav/Shells/Id3Conv/id3iconv-0.2.1.jar
PREID3=gb2312

if [ $# -eq 1 ];then
    java -jar $ID3CONV -e $PREID3 $1
    exit
fi

find . -name "*.[Mm][Pp]3" -exec java -jar $ID3CONV -e $PREID3 {} \;


