TEMPLATE = app
TARGET = Gomoku_bak
QT += core \
    gui
HEADERS += Gui/MainWindow.h \
    Gui/Engine.h \
    core/statement.h \
    core/gomokuCore.h
SOURCES += Gui/MainWindow.cpp \
    Gui/Engine.cpp \
    core/gomokuCore.cpp \
    main.cpp
FORMS += 
RESOURCES += 
