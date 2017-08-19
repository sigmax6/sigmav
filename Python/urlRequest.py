#!/usr/bin/python
################################
#PYTHON TEMPLATE FOR SIGMAX6
#
#
#
################################

import urllib.request

page = urllib.request.urlopen("http://www.baidu.com")
text = page.read().decode("utf8")

print(text)

