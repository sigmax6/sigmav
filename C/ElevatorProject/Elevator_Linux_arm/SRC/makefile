#
# Makefile for Linux application test example.
#
#---------------------------------------------------------------
# Linux application Makefile sample
# make          -- the optical output without debug info
# make DEBUG=1  -- debug info include
# make clean    -- clean the object file(s)
# make install  -- install the execute file(s) to INSTALLDIR
#---------------------------------------------------------------
# History£º
#			Program start, Embest	R.X.Huang 2005.6.5

LINUXDIR	= /usr/local/src/edukit-2410/kernel
CROSSDIR	= /usr
INSTALLDIR	= /home/app

#---------------------------------	/* execute file(s) */
TESTFILE 	= elevator
#---------------------------------	/* object file(s) */
SRCFILE		= elevator.c key.c led.c
#---------------------------------	/* header file(s) */
TESTFILE_H	= key.h

CROSS=arm-linux-
CC = $(CROSS)gcc
AS = $(CROSS)as
LD = $(CROSS)ld

ifdef DEBUG
CFLAGS = -g
else
CFLAGS = -O2 -fomit-frame-pointer
endif

CFLAGS += -mapcs-32 -march=armv4 -mtune=arm9tdmi -fno-builtin

INCLUDES = -I$(LINUXDIR)/include \
			-I$(CROSSDIR)/arm-linux/include \
			-I$(CROSSDIR)/lib/gcc-lib/arm-linux/2.95.3/include 
CFLAGS +=$(INCLUDES)

CRYPT    =  $(CROSSDIR)/arm-linux/lib/crt1.o \
			$(CROSSDIR)/arm-linux/lib/crti.o \
			$(CROSSDIR)/arm-linux/lib/crtn.o

LIBDIR   = -L$(CROSSDIR)/arm-linux/lib/ \
			-L$(CROSSDIR)/lib/gcc-lib/arm-linux/2.95.3 

LDFLAGS += $(LIBDIR) -nostartfiles -static

all:   $(TESTFILE)

$(TESTFILE):  $(SRCFILE) $(TESTFILE_H) Makefile 
	$(CC) $(CFLAGS) -o $@ $@.c -lpthread

clean:
	rm -f $(TESTFILE)
	
install: $(TESTFILE)
	mkdir -p $(INSTALLDIR)
	cp --target-dir=$(INSTALLDIR) $(TESTFILE)
	cp i2c.o $(INSTALLDIR)
