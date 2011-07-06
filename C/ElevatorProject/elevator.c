#include<stdio.h>
#include<pthread.h>
int eleorder[4]={0};
void getkey(void)
{
  int key;
  while(1)
  {
    //key=getchar()-'0';
    scanf("%d",&key);
    if(key>=1&&key<=4&&eleorder[key-1]!=1)
    {
        eleorder[key-1]=1;
        printf("#######%d",key);
    }
  }
}
void eprint(void)
{
  int elefloor=1;             // floor
  int elestate=0;              // 0 closed 1 open
  int elefloorarr[4]={0};
  int upordown=1;              // 1 up 0 down;
  int stopped=0;               // 1 stop
  int elefloortemp;
  int i;
  int j;
  while(1)
  {
    if(1==upordown)
     {
        for(i=elefloor;i<4;i++)
        {
          if(eleorder[i]==1) break;
        }
        if(i>=4)
        {
            upordown=0;
            for(i=elefloor-2;i>=0;i--)
            {
               if(eleorder[i]==1) break;
            }
            if(i<0)
               stopped=1;
        }
     }
     else
     {
        for(i=elefloor-1;i>=0;i--)
        {
          if(eleorder[i]==1) break;
        }
        if(i<0)
        {
            upordown=1;
            for(i=elefloor;i<4;i++)
            {
               if(eleorder[i]==1) break;
            }
            if(i==4)
               stopped=1;
        }
     }
     elefloortemp=elefloor;
    if(!stopped)
    {
     elefloor=i+1;
     while(elefloortemp!=elefloor)
     {
       for(i=0;i<4;i++)
       {
          elefloorarr[i]=0;
       }
       elefloorarr[elefloortemp-1]=1;
       //print
//       system("clear");
       printf(" 1 2 3 4\n");
       printf("%2d%2d%2d%2d\n",elefloorarr[0],elefloorarr[1],elefloorarr[2],elefloorarr[3]);
       printf("%2d%2d%2d%2d\n",eleorder[0],eleorder[1],eleorder[2],eleorder[3]);
       (upordown==1)?elefloortemp++,printf("running up\n"):elefloortemp--,printf("running down\n");
       //
//       for(i=0;i<100000;i++)
//       {
 //      }
     }
     //
      for(i=0;i<4;i++)
       {
          elefloorarr[i]=0;
       }
       elefloorarr[elefloor-1]=1;
     //
//     system("clear");
     printf(" 1 2 3 4\n");
     printf("%2d%2d%2d%2d\n",elefloorarr[0],elefloorarr[1],elefloorarr[2],elefloorarr[3]);
     printf("%2d%2d%2d%2d\n",eleorder[0],eleorder[1],eleorder[2],eleorder[3]);
     printf("Door opened!\n");
     //
 //    for(i=0;i<100000;i++)
 //    {
 //    }
     //
 //    system("clear");
     printf(" 1 2 3 4\n");
     printf("%2d%2d%2d%2d\n",elefloorarr[0],elefloorarr[1],elefloorarr[2],elefloorarr[3]);
     printf("%2d%2d%2d%2d\n",eleorder[0],eleorder[1],eleorder[2],eleorder[3]);
     printf("Door closed!\n");
     //
 //    for(i=0;i<100000;i++)
 //    {
 //    }
    }
    else
    {
     //
  //   system("clear");
     printf(" 1 2 3 4\n");
     printf("%2d%2d%2d%2d\n",elefloorarr[0],elefloorarr[1],elefloorarr[2],elefloorarr[3]);
     printf("%2d%2d%2d%2d\n",eleorder[0],eleorder[1],eleorder[2],eleorder[3]);
     printf("Waiting!\n");
     //
    }
    eleorder[elefloor-1]=0;
  }
 }
int main(int argc,char** argv)
{
   pthread_t getkey_t,print_t;
   pthread_create(&getkey_t,NULL,(void *)&getkey,NULL);
   pthread_create(&print_t,NULL,(void *)&eprint,NULL);
   return 0;
}
