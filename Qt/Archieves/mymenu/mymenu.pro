#-------------------------------------------------
#
# Project created by QtCreator 2010-08-15T19:36:13
#
#-------------------------------------------------

QT       += core gui

CONFIG += qtopiaapp
CONFIG -= buildQuicklaunch
DESTDIR =$(QPEDIR)/bin

TARGET = mymenu
TEMPLATE = app


SOURCES += main.cpp\
        menu.cpp \
    mypage.cpp \
    qextserialbase.cpp \
    win_qextserialport.cpp

HEADERS  += menu.h \
    mypage.h \
    qextserialbase.h \
    win_qextserialport.h

FORMS += \
    mymenu.ui

RESOURCES += \
    rsc.qrc

OTHER_FILES += \
    zlhgr.jpg \
    zgdxj.jpg \
    xglc.jpg \
    srfssb.jpg \
    sbj.jpg \
    lkjc.jpg \
    djly.jpg \
    dazj.jpg
