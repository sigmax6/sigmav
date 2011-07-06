#!/bin/sh
# /etc/acpi/default.sh
# Default acpi script that takes an entry for all actions
# vim: set ts=4 sw=4:

set $*

group=${1%%/*}
action=${1#*/}
device=$2
id=$3
value=$4

if [ "$group" = "ibm" ]
then
	key="$2"\ "$3"\ "$4"
fi

log_unhandled() {
	logger "ACPI event unhandled: $*"
}

case "$group" in
	button)
		case "$action" in
			power)
				logger "acpid: *power* button pressed;"
				/sbin/init 0
				;;
			lid)
				logger "acpid: *lid* event triggered;"
				/etc/acpi/lid.sh
				;;
			*)	log_unhandled $* ;;
		esac
		;;
	ibm)
		case "$action" in
			hotkey)
				case "$key" in
					"HKEY 00000080 00001002")
						logger "acpid: *lock* button (Fn+F2) pressed;"
						/etc/acpi/lock.sh
						;;
					"HKEY 00000080 00001003")
						logger "acpid: *battery* button (Fn+F3) pressed;"
						/etc/acpi/display.sh
						;;
					"HKEY 00000080 00001004")
						logger "acpid: *sleep* button (Fn+F4) pressed;"
						/usr/sbin/hibernate-ram
						;;
					"HKEY 00000080 00001005")
						logger "acpid: *radio* button (Fn+F5) pressed;"
						/etc/init.d/net.ppp0 start
						;;
					"HKEY 00000080 00001007")
						logger "acpid: *screen* button (Fn+F7) pressed;"
						/etc/acpi/video-out.sh
						;;
					"HKEY 00000080 00001008")
						logger "acpid: *touchpoint* button (Fn+F8) pressed;"
						#TBD
						;;
					"HKEY 00000080 00001009")
						logger "acpid: *eject* button (Fn+F9) pressed;"
						/usr/local/bin/ultrabase_eject
						;;
					"HKEY 00000080 0000100c")
						logger "acpid: *hibernate* button (Fn+F12) pressed;"
						/usr/sbin/hibernate
						;;
					"HKEY 00000080 00001010")
						logger "acpid: *brightness up* button (Fn+Home) pressed;"
						echo up > /proc/acpi/ibm/brightness
						;;
					"HKEY 00000080 00001011")
						logger "acpid: *brightness down* button (Fn+End) pressed;"
						echo down > /proc/acpi/ibm/brightness
						;;
					"HKEY 00000080 00001014")
						logger "acpid: *zoomin* button pressed;"
						/usr/local/bin/acpi_fakekey 183
						;;
					"HKEY 00000080 00001018")
						logger "acpid: *thinkvantage* button pressed;"
						#TBD
						;;
					"HKEY 00000080 00007000")
						logger "acpid: *radio* switch toggled;"
						/etc/acpi/wireless.sh
						;;
					*)	log_unhandled $* ;;
				esac
				;;
			*)	log_unhandled $* ;;
		esac
		;;
	*)	log_unhandled $* ;;
esac
