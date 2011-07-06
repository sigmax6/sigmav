CONFIG		+= qtopiaapp
CONFIG		-= buildQuicklaunch

HEADERS	= template.h
SOURCES	= template.cpp

SOURCES+=main.cpp

INTERFACES	= template_base.ui

desktop.files = template.desktop
desktop.path = /apps/Applications
INSTALLS += desktop

pics.files=pics/*
pics.path=/pics/Games
PICS_INSTALLS+=pics

TARGET		= template
