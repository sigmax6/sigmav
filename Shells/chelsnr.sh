#!/bin/bash
. /home/oracle/.bash_profile
declare -i n_lsnr
n_lsnr=`ps -ef|grep LISTENER|grep -v grep|wc -l`
if [ $n_lsnr = 0 ] ; then
    lsnrctl start>/dev/null
    echo "Listener is down,and restarted."
fi
