#include<stdio.h>
int main(int argc,char *argv[])
{
    FILE * fSrc,fDst;
    fSrc=fopen(argv[1],"rb");
    if(NULL==fSrc)
    {
        printf("Open file %s failed!\n",argv[1]);
        return -1;
    }
    fDst=fopen(argv[2],"wb");
    if(NULL==fDst)
    {
        printf("Open file %s failed!\n",argv[2]);
        return -1;
    }
}