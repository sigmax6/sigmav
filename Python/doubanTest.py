#!/bin/python        
# -*- coding: UTF8 -*-

import urllib
import httplib
import json

f = urllib.urlopen('http://www.douban.com/j/app/radio/channels')
data = f.read()
f.close()
channels = json.loads(data)
#self.channels = {}
for channel in channels['channels']:
    print channel['name_en']
    print channel['channel_id'] 
