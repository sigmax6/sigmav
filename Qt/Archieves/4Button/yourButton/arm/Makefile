#############################################################################
# Makefile for building $(PWD)/yourbutton
# Generated by tmake at 09:09, 2010/07/20
#     Project: yourbutton
#    Template: app
#############################################################################

####### Compiler, tools and options

CC	=	arm-linux-gcc
CXX	=	arm-linux-g++
CFLAGS	=	-pipe -Wall -W -O2 -DNO_DEBUG
CXXFLAGS=	-pipe -DQT_QWS_IPAQ -DQWS -fno-exceptions -fno-rtti -Wall -W -O2 -DNO_DEBUG
INCPATH	=	-I$(QTDIR)/include
LINK	=	arm-linux-gcc
LFLAGS	=	
LIBS	=	$(SUBLIBS) -L$(QTDIR)/lib -lqte
MOC	=	$(QTDIR)/bin/moc
UIC	=	$(QTDIR)/bin/uic

TAR	=	tar -cf
GZIP	=	gzip -9f

####### Files

HEADERS =	yourbutton.h
SOURCES =	yourbutton.cpp \
		main.cpp
OBJECTS =	yourbutton.o \
		main.o \
		your_button_base_form.o
INTERFACES =	your_button_base_form.ui
UICDECLS =	your_button_base_form.h
UICIMPLS =	your_button_base_form.cpp
SRCMOC	=	moc_yourbutton.cpp \
		moc_your_button_base_form.cpp
OBJMOC	=	moc_yourbutton.o \
		moc_your_button_base_form.o
DIST	=	
TARGET	=	$(PWD)/yourbutton
INTERFACE_DECL_PATH = .

####### Implicit rules

.SUFFIXES: .cpp .cxx .cc .C .c

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules


all: $(TARGET)

$(TARGET): $(UICDECLS) $(OBJECTS) $(OBJMOC) 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(LIBS)

moc: $(SRCMOC)

tmake: Makefile

Makefile: yourbutton.pro
	tmake yourbutton.pro -o Makefile

dist:
	$(TAR) yourbutton.tar yourbutton.pro $(SOURCES) $(HEADERS) $(INTERFACES) $(DIST)
	$(GZIP) yourbutton.tar

clean:
	-rm -f $(OBJECTS) $(OBJMOC) $(SRCMOC) $(UICIMPLS) $(UICDECLS) $(TARGET)
	-rm -f *~ core

####### Sub-libraries


###### Combined headers


####### Compile

yourbutton.o: yourbutton.cpp \
		yourbutton.h \
		your_button_base_form.h

main.o: main.cpp \
		yourbutton.h \
		your_button_base_form.h

your_button_base_form.h: your_button_base_form.ui
	$(UIC) your_button_base_form.ui -o $(INTERFACE_DECL_PATH)/your_button_base_form.h

your_button_base_form.cpp: your_button_base_form.ui
	$(UIC) your_button_base_form.ui -i your_button_base_form.h -o your_button_base_form.cpp

your_button_base_form.o: your_button_base_form.cpp \
		your_button_base_form.h \
		your_button_base_form.ui

moc_yourbutton.o: moc_yourbutton.cpp \
		yourbutton.h \
		your_button_base_form.h

moc_your_button_base_form.o: moc_your_button_base_form.cpp \
		your_button_base_form.h

moc_yourbutton.cpp: yourbutton.h
	$(MOC) yourbutton.h -o moc_yourbutton.cpp

moc_your_button_base_form.cpp: your_button_base_form.h
	$(MOC) your_button_base_form.h -o moc_your_button_base_form.cpp

