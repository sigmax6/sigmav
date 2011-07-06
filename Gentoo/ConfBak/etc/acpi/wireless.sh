#!/bin/sh
if [ `cat /sys/devices/platform/thinkpad_acpi/rfkill/rfkill1/state` == "1" ]
then
	# Do not establish any wireless connection by default
	echo disable > /proc/acpi/ibm/bluetooth
else
	# Terminate all wireless connections
	/etc/init.d/net.wlan0 stop
	/etc/init.d/net.ppp0 stop
	/etc/init.d/f3507g stop
fi
