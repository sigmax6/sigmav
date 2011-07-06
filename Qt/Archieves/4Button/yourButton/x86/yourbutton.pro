CONFIG		+= qtopiaapp
CONFIG		-= buildQuicklaunch
DESTDIR	= $(PWD)

HEADERS	= yourbutton.h
SOURCES	= yourbutton.cpp

INTERFACES = your_button_base_form.ui

SOURCES+=main.cpp

desktop.files = yourbutton.desktop
desktop.path = /apps/Applications
INSTALLS += desktop

pics.files=pics/*
pics.path=/pics/Games
PICS_INSTALLS+=pics

TARGET		= yourbutton
