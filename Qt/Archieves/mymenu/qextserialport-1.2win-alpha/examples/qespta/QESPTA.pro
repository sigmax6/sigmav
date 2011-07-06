######################################################################
# QextSerialPort Test Application (QESPTA)
######################################################################


PROJECT = QESPTA
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += ../..
QMAKE_LIBDIR += ../../build


OBJECTS_DIR    = obj
MOC_DIR        = moc
UI_DIR         = uic
CONFIG      += qt thread warn_on


HEADERS += MainWindow.h \
		MessageWindow.h \
		QespTest.h \
    qextserialport.h \
    qextserialenumerator.h \
    qextserialbase.h \
    win_qextserialport.h

SOURCES += main.cpp \
		MainWindow.cpp \
		MessageWindow.cpp \
		QespTest.cpp \
    win_qextserialport.cpp \
    qextserialport.cpp \
    qextserialenumerator.cpp \
    qextserialbase.cpp

CONFIG(debug, debug|release):LIBS  += -lqextserialportd
else:LIBS  += -lqextserialport

unix:DEFINES   = _TTY_POSIX_
win32:DEFINES  = _TTY_WIN_
