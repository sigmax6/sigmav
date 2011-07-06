#!/bin/bash
# A sh to log the cpu idle rate to $log file
# @ sigmav.net

# Prepare the files
logdate=`date +%F`;               #The name of the log YYYY-MM-DD
# log=`/var/log/$logdate`;         #Path of the log file

# Calc the cpu idle rate
# Cpu idle rate=100*idle/(CPUTIME=user+nice+system+idle+iowait+irq+softirq)
# Set two time point to take the idle1 idle2 && CPUTIME1 CPUTIME2
# The avg is 100*(idle2-idle1)/(CPUTIME2-CPUTIME1)

idle1=`head -1 /proc/stat | awk '{print $5}'`;
CPUTIME1=`head -1 /proc/stat | awk '{print $2+$3+$4+$5+$6+$7+$8}'`;

# Sleep 10 s to reach timepoint 2
sleep 10;
# End of Sleep

idle2=`head -1 /proc/stat | awk '{print $5}'`;
CPUTIME2=`head -1 /proc/stat | awk '{print $2+$3+$4+$5+$6+$7+$8}'`;

idle=$[$idle2-$idle1];
CPUTIME=$[$CPUTIME2-$CPUTIME1];

# Use bd to calc the float numbers
idlerate=$(echo "scale=2;100*$idle/$CPUTIME"|bc);

# Set the log time info as ?H-mm-idlerate
timeinfo=`date +%k-%M-`;

# Out put log file
if test -f /var/log/$logdate; then
     echo "$timeinfo$idlerate" >> /var/log/$logdate;
else
     echo "$timeinfo$idlerate" > /var/log/$logdate;
fi
