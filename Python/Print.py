#!/usr/bin/python
print "his name is %(name)s,his weight is %(weight)s" % {"name":"tom","weight":"180"}

def func(a,b):
    print "a:%(a)d,b:%(b)d" %locals()

func(1,2)
