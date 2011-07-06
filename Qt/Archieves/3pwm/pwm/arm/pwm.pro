CONFIG		+= qtopiaapp
CONFIG		-= buildQuicklaunch

HEADERS	= pwm.h
SOURCES	= pwm.cpp

SOURCES+=main.cpp

INTERFACES	= pwm_base.ui

desktop.files = pwm.desktop
desktop.path = /apps/Applications
INSTALLS += desktop

pics.files=pics/*
pics.path=/pics/Games
PICS_INSTALLS+=pics

TARGET		= pwm
