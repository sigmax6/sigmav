TARGET=Cemara
CONFIG+=qt
HEADERS+=Cemara.h
SOURCES+=Cemara.cpp
SOURCES+=main.cpp
INCLUDEPATH += . \
    /usr/include/opencv
LIBS += /usr/lib/libcv.so \
    /usr/lib/libcvaux.so \
    /usr/lib/libcxcore.so \
    /usr/lib/libhighgui.so \
    /usr/lib/libml.so
