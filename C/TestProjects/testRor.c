#include<stdio.h>

union testEnding
{
    long tester;
    char test[2];
}testE;
int systemBits =8*sizeof(size_t);
int ror(int a,int n)
{
   int b=a<<(systemBits-n);
   int c=a>>n;
   c=c|b;
   return c;
}
int main (int argc, char * argv[])
{
   int a,n;
   printf("You are using a System of %d Bits\n",systemBits);
   testE.tester=0x1122;
   if(testE.test[0]==0x22)
   {
       printf("This is a big Ending System\n");
   }
   else if(testE.test[0]==0x11)
   {
       printf("This is a little Ending System\n");
   }
   printf("Input the number you want to ror:\n");
   scanf("%x",&a);
   printf("Input the bits you want to ror:\n");
   scanf("%d",&n);
   a=ror(a,n);
   printf("After ror it is: %x\n",a);
   return 0;
}

