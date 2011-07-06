#!/bin/bash
# Usage : airsh <app-name>
#
#

AIR_PATH=/opt/AIR_SDK/bin/adl
APP_PATH=/opt/AIR_APP

if [ $# != 1 ] ; then
    echo "Usage $0 <app-name>"
else
    ${AIR_PATH} -nodebug "${APP_PATH}/$1/META-INF/AIR/application.xml" "${APP_PATH}/$1"
fi
