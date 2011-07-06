#!/usr/bin/python
def getTure():
    print "debug:ture"
    return 1

def getFalse():
    print "debug:false"
    return 0

if getTure() and getFalse():
    print "OK"
else:
    print "bad"

x= getTure() and "ture" or "false"
print x

x= getFalse() and "ture" or "false"
print x
