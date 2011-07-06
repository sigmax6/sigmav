CONFIG		+= qtopiaapp
CONFIG		-= buildQuicklaunch

HEADERS	= adc.h
SOURCES	= adc.cpp

SOURCES+=main.cpp

INTERFACES	= adc_base.ui

desktop.files = adc.desktop
desktop.path = /apps/Applications
INSTALLS += desktop

pics.files=pics/*
pics.path=/pics/Games
PICS_INSTALLS+=pics

TARGET		= adc
