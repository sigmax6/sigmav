# Makefile  
# Please modify here or set environments.
# KSOURCE should be pointed to the build directory of your kernel.
#
DEBUG ?= n
KSOURCE ?= /utuLinux/utu-Linux2.6.24_for_utu2440_2009-07-18

%.x:%.c
	arm-linux-gcc -o $@ $<

KBUILD_VERBOSE:=1

obj-m := nrf24l01.o

default:
	make -C $(KSOURCE) LANG=C KBUILD_VERBOSE=${KBUILD_VERBOSE}  SUBDIRS=`pwd` modules

.PHONY: cscope
cscope:
	cscope -b -k -R

.PHONY: clean
clean:
	make -C $(KSOURCE) LANG=C KBUILD_VERBOSE=${KBUILD_VERBOSE}  SUBDIRS=`pwd` clean
	rm -f *.x *~
