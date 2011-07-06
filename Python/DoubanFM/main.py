#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, os, time, thread, glib, gobject
import gst, pygst
import json
import urllib
import libdbfm
import getpass
from select import select

class DoubanFM_Player:
    def __init__(self):
        self.username = raw_input("请输入豆瓣用户名:")
        self.password = getpass.getpass("请输入豆瓣密码:")
        self.use_info = u'''
开始使用： n 下一首 |f 加心 |u 取消加心 |d 删除歌曲 |s 跳过歌曲 |l 切换频道 |h 帮助>>
'''
        print self.use_info
                         
        try:
            self.user = libdbfm.DoubanFM(self.username,self.password)
        except:
            print "登录异常，请重试"
            sys.exit()

        self.player = gst.element_factory_make("playbin", "player")
        bus = self.player.get_bus()
        bus.add_signal_watch()
        bus.connect("message", self.on_message)

    def channel_info(self):
        f = urllib.urlopen('http://www.douban.com/j/app/radio/channels')
        data = f.read()
        f.close()
        channels = json.loads(data)
        print "*频道序号*\t|*频道名称*\t"
        for channel in channels['channels']:
            print channel['channel_id'], '\t|', channel['name'], '\t'
        print ">>当前频道:\t|", self.user.channel, '\t'

    def on_message(self, bus, message):
        t = message.type
        if t == gst.MESSAGE_EOS:
            self.player.set_state(gst.STATE_NULL)
            self.playmode = False
        elif t == gst.MESSAGE_ERROR:
            self.player.set_state(gst.STATE_NULL)
            err, debug = message.parse_error()
            print u"播放错误 %s" % err, debug
            self.playmode = False

    def get_songlist(self):
        if self.user:
            self.songlist=self.user.new_playlist()
        else:
            print u"请先登录"

    def control(self, r):
        rlist, _, _ = select([sys.stdin], [], [], 1)
        if rlist:
            s = sys.stdin.readline()
            if s[0] == 'n':
                return 'next'
            elif s[0] == 'f':
                self.user.fav_song(r['sid'], r['aid'])
                print u"歌曲加心成功"
                return 'fav'
            elif s[0] == 'd':
                self.songlist = self.user.del_song(r['sid'], r['aid'])
                print u"歌曲删除成功"
                return 'del'
            elif s[0] == 'u':
                self.user.unfav_song(r['sid'], r['aid'])
                print u"取消加心成功"
                return 'unfav'
            elif s[0] == 's':
                self.songlist = self.user.skip_song(r['sid'], r['aid'])
                print u"跳过歌曲成功"
                return 'skip'
            #elif s[0] == 'q':
                #print "确认退出程序吗？[y/n]"
                #q = sys.stdin.readline()
                #if q[0] == 'y':
                #    return 'quit'
                #else:
                #    print "取消退出操作"
            elif s[0] == 'l':
                self.channel_info()
                l = raw_input("输入新频道序号:")
                if l == self.user.channel:
                    print "未更改频道 >> "
                    return 'unchange_list'
                else:
                    self.user.channel = l
                    print "已更新频道 >> "
                    print self.user.channel
                    return 'change_list'
            elif s[0] == 'h':
                print self.use_info

    def start(self):
        self.get_songlist()
        for r in self.songlist:
            song_uri = r['url']
            self.playmode = True

            if r['like'] == '0':
                self.like = u''
            else:
                self.like = u'[加心歌曲]'

            print u'正在播放： '+self.like+r['title']+u'    歌手： '+r['artist']
            self.player.set_property("uri",song_uri)
            self.player.set_state(gst.STATE_PLAYING)
            while self.playmode:
                c = self.control(r)
                if c == 'next' or c == 'del' or c == 'skip':
                    self.player.set_state(gst.STATE_NULL)
                    self.playmode = False
                    break
                elif c == 'quit':
                    self.player.set_state(gst.STATE_NULL)
                    self.playmode = False
                    flag_quit = True
                    print "正在退出，感谢使用"
                    break 
                elif c == 'change_list':
                    self.player.set_state(gst.STATE_NULL)
                    self.playmode = False
                    print "切换列表"
                    loop.quit()
        
        loop.quit()
        

print u"欢迎使用豆瓣电台"

dbplayer = DoubanFM_Player()

flag_quit = False

while 1:
    thread.start_new_thread(dbplayer.start, ())
    gobject.threads_init()
    loop = glib.MainLoop()
    loop.run()

