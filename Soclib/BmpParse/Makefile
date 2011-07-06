all:lena lenaTest lenaRes

lena:lena.cpp EasyBMP.cpp
	g++ -Wall -o lena lena.cpp EasyBMP.cpp
lenaRes:lenaRes.cpp EasyBMP.cpp
	g++ -Wall -o lenaRes lenaRes.cpp EasyBMP.cpp
lenaTest:lena lenaTest.c 
	./lena
	gcc -Wall -o lenaTest lenaTest.c
clean:
	rm lena lenaTest lenaRes
