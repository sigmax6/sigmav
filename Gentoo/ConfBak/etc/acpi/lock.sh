#!/bin/sh

LOCK_FILE=/tmp/tp-antitheft.lock

touch $LOCK_FILE
/usr/local/bin/tp-antitheft &
vlock -n &&
pkill tp-antitheft
rm -f $LOCK_FILE
