#include<stdio.h>

union test
{
    unsigned char cg[4];
    float b;
}Test;

int main()
{
     int i;
	//float b;
	//unsigned char c1=10;
	unsigned char c=1;
	//char d;
	Test.b=(float)c;
	//Test.b=1;
	printf("%d\n",Test.b);
	for(i=3;i>=0;i--)
	{
	    printf("%x  ",Test.cg[i]);
	}
}
