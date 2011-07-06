#include <stdio.h>
#include <unistd.h>
#define BUFF_SIZE 8192

int main (int argc, char const* argv[])
{
   int n;
   char buff[BUFF_SIZE];

   while ((n = read(STDIN_FILENO,buff,BUFF_SIZE)) > 0)
   {
       if(write(STDOUT_FILENO,buff,n) != n)
       {
           printf("write error!\n");
           return -1;
       }
   }

   if(n < 0)
   {
       printf("read error\n");
       return -1;
   }

   return 0;
}

