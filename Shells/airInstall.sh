#!/bin/bash
if [[ $# != 1 ]]
then
    print "Usage:$0 <FILENAME.air>"
    exit
fi

STR=${1##*\/}
NAME=${STR%%.*}
AIR=${STR##*.}

if [[ $AIR != 'air' ]]
then
    print "$1 is not a *.air file"
    exit
fi

mkdir /opt/$NAME
unzip $1 -d /opt/Adobe/Air/$NAME/
echo "/opt/Adobe/Air/bin/adl -nodebug /opt/Adobe/Air/$NAME/META-INF/AIR/application.xml /opt/Adobe/Air/$NAME/" > /home/sigmax6/.bin/$NAME
chmod +x /home/sigmax6/.bin/$NAME
