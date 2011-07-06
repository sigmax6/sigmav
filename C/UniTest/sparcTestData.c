#include <stdio.h>
int main (int argc, char const* argv[])
{
    FILE * stream;
   int i=0;
   int counter=0;
   if((stream=fopen("cfg","w"))==NULL)
    {
        printf("open file error!\n");
        return -1;
    }
   fprintf(stream,"wmem 0x80000000 0xf800ff\nmcfg1 0xf800ff\nwmem 0x80000004 0xc4f\nmcfg2 0xc4f\nstack 0x40070000\n");
   for(i=0;i<0xFFFFF;i+=4)
   {
     fprintf(stream,"wmem 0x4%.7x 0x12345678\n",i);
     counter++;
     if(counter==4)
     {
         fprintf(stream,"mem 0x4%.7x 10\n",i-12);
         counter=0;
     }
   }
   return 0;
}

