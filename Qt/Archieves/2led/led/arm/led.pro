CONFIG		+= qtopiaapp
CONFIG		-= buildQuicklaunch

HEADERS	= led.h
SOURCES	= led.cpp

SOURCES+=main.cpp

INTERFACES	= led_base.ui

desktop.files = led.desktop
desktop.path = /apps/Applications
INSTALLS += desktop

pics.files=pics/*
pics.path=/pics/Games
PICS_INSTALLS+=pics

TARGET		= led
