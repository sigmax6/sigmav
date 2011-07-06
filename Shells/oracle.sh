#!/bin/bash
echo "Oracle database configure script by Sigmax6 2010"
case "$1" in 
    start)
    echo "Starting Oracle Listener and Database .... "
    echo "--------------------------------------------">>/var/log/oracle
    data+"%T %a %D:Starting Oracle Listener and Database...">>/var/log/oracle
    echo "--------------------------------------------">>/var/log/oracle
    su -oracle -c dbstartup >> /var/log/oracle
    echo "Done"
    echo ""
    echo "--------------------------------------------">>/var/log/oracle
    data+"%T %a %D:Finisted.">> /var/log/oracle
    echo "--------------------------------------------">>/var/log/oracle
    touch /var/lock/subsys/oracle
    ;;
    stop)
    echo "Stop Oracle Database and Listener ..."
    echo "--------------------------------------------">>/var/log/oracle
    data+"%T %a %D:Stop Oracle Database and Linster...">> /var/log/oracle
    echo "--------------------------------------------">>/var/log/oracle
    su -oracle -c dbstop >> /var/log/oracle
    rm -f /var/lock/subsys/oracle
    echo "Done"
    echo ""
    echo "--------------------------------------------">>/var/log/oracle
    data+"%T %a %D:Finisted.">> /var/log/oracle
    echo "--------------------------------------------">>/var/log/oracle
    ;;
    restart)
    echo "Restarting Oracle Listener and Database..."
    echo "--------------------------------------------">>/var/log/oracle
    data+"%T %a %D:Restart Oracle Database and Linster...">> /var/log/oracle
    echo "--------------------------------------------">>/var/log/oracle
    su -oracle -c dbstop >> /var/log/oracle
    su -oracle -c dbstartup >> /var/log/oracle
    echo "Done"
    echo ""
    echo "--------------------------------------------">>/var/log/oracle
    data+"%T %a %D:Finisted.">> /var/log/oracle
    echo "--------------------------------------------">>/var/log/oracle
    touch /var/lock/subsys/oracle
    ;;
    *)
    echo "Usabe : oracle {start|stop|restart}"
    exit 1
esac
