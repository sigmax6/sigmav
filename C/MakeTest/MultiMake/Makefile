#MutiMake Test
# a.c
# sub1/ b.c
# sub2/ c.c

Files=a.o b.o c.o

all:$(Files)
	gcc -o A a.o
	gcc -o B b.o
	gcc -o C c.o

A: a.o
	gcc -o A a.o
B: b.o
	gcc -o B b.o
C: c.o
	gcc -o C c.o

a.o: a.c
	gcc -c  a.c
b.o: sub1/b.c
	gcc -c  sub1/b.c
c.o: sub2/c.c
	gcc -c  sub2/c.c

.PHONY: clean
clean: 
	-rm A B C $(Files)
