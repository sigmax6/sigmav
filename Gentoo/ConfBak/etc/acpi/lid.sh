#!/bin/sh

if [ `cat /proc/acpi/button/lid/LID/state|awk '{print $2}'` == "open" ]
then
	exit 0
else
	if [ ! -e /tmp/tp-antitheft.lock ] && [ ! -e /tmp/display-dpms-off.lock ]
	then
		/usr/sbin/hibernate-ram --force
	fi
fi
