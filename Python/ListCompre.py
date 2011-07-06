#!/usr/bin/python

a=['123','345','456','abc','def','gHI']

b=[k.center(9) for k in a]

print b

b=[k.upper() for k in a if k.isalpha()]

print b
