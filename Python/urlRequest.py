#!/usr/bin/python
################################
#PYTHON TEMPLATE FOR SIGMAX6
#
#
#
################################

import urllib

page = urllib.urlopen("http://news.google.com")
text = page.read().decode("utf8")

print(text)

