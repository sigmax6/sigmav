CONFIG		+= qtopiaapp
CONFIG		-= buildQuicklaunch

HEADERS	= template.h
SOURCES	= template.cpp

SOURCES+=main.cpp


desktop.files = template.desktop
desktop.path = /apps/Applications
INSTALLS += desktop

pics.files=pics/*
pics.path=/pics/Games
PICS_INSTALLS+=pics

TARGET		= template
