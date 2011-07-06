#!/usr/bin/python
################################
#PYTHON TEMPLATE FOR SIGMAX6
#
#
#
################################

#print("Helloworld!")

msg = "<HTML> Just a test string !"

print(msg.upper())
if msg.endswith("!"):
    print("end with !")
print(msg.endswith("!"))
print(msg.lower())
print(msg.replace("a","some"))
print(msg.strip())
print(msg.find("test"))
print(msg.startswith("<HTML>"))
if msg.startswith("<HTML>"):
    print("start with <HTML>")

