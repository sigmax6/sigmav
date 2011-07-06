CONFIG		+= qtopiaapp
CONFIG		-= buildQuicklaunch
DESTDIR	= $(PWD)

HEADERS	= yourbutton.h
SOURCES	= yourbutton.cpp

INTERFACES = your_button_base_form.ui

SOURCES+=main.cpp

TARGET		= yourbutton
