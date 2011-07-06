#!/bin/bash
lsnrctl start
sqlplus / nolog <<EOF
connect / as sysdba
startup
EOF
