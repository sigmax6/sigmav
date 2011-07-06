#!/usr/bin/python
# -*- coding:UTF-8 -*-

import os,sys
print "新增任务:"
task = sys.stdin.readline().strip('\n')
task = "rainlendar -a \""+task+"\" 1>/dev/null 2>&1" 
os.system(task)
#if os.system(task) == 0:
#    print "任务增加成功！"
#else:
#    print "任务增加失败!"

