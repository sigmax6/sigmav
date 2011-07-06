#!/bin/bash
. /home/oracle/.bash_profile
sqlplus -s system/oracle  << EOF
set feed off
set verify off
set heading off
set pagesize 0
set linesize 300
select f.tablespace_name,f.free_space/d.total_space free_percen
from 
(select tablespace_name,sum(bytes) free_space from dba_free_space group by tablespace_name) f,
(select tablespace_name,sum(bytes) total_space from dba_data_files group by tablespace_name) d
where f.tablespace_name=d.tablespace_name and f.free_space/d.total_space<0.20
/
exit
EOF
