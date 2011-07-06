#!/bin/sh

LOCK_FILE=/tmp/display-dpms-off.lock

if [ -e $LOCK_FILE ]
then
	vbetool dpms on
	rm -f $LOCK_FILE
else
	touch $LOCK_FILE
	vbetool dpms off
fi
