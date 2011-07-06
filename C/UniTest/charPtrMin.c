#include <stdio.h>

int main (int argc, char const* argv[])
{
   typedef char * CHARPTR;
   CHARPTR pc1,pc2;
   pc1=(char *)100;
   pc2=(char *)200;
   printf("%d",(pc2-10)-pc1);
   return 0;
}

