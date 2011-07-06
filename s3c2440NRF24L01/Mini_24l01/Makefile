#Mini2440 General Makefile for SSDUT_MCU_LAB
#Auth: sigmax6 
#2011 03 01

CC = arm-linux-gcc                                 #name of platform compiler
KDIR ?= /opt/FriendlyArm/mini2440/linux-2.6.32.2    #platform linux source dir 
PWD := $(shell pwd)

obj-m := nrf24l01.o                                   #obj-m means hello.ko , and it depends on hello.o

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules            #make 

.PHONY: clean
clean:                                               
	$(MAKE) -C $(KDIR) M=$(PWD) clean              #clean
	rm -rf *.o *.ko .*.cmd *.mod.*

