# Makefile for 8Queen 
# Make debug      编译调试版本
# Make            编译优化调整版本
# Make details    编译保留细节版本
# Make clean      清除构造版本

CC = gcc
AS = as

EXEC=8Queen
OBJS=8Queen.c

MACRO=

ifdef DEBUG
	ASFLAGS=     #调试操作
else
	CFLAGS=-O2   #执行代码优化
endif

INCLUDES=
CRYPT=

LIBDIR=

all:compile

compile:$(EXEC)
	$(CC) -o $(EXEC) $(OBJS)
debug:$(EXEC)
	$(CC) -g -o $(EXEC) $(OBJS)
clean:
	rm $(EXEC) 

#END OF MAKEFILE
