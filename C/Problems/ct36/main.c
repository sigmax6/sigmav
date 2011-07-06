#include<stdio.h>
#define MAX 100001
int main(void)
{
    int flagMap[MAX]={0};
    int m,n,counter,tcounter,max,tmax;
    while(scanf("%d%d",&m,&n)!=EOF)
    {
        m>n?m=m^n,n=m^n,m=m^n:0;
        max=tmax=0;
        for(counter=m;counter<=n;counter++)
        {
          tmax=0;
          if(flagMap[counter]==0)
          {
            tcounter=counter;
            while(tcounter!=1)
            {
             if(!(tcounter&1))
             {
                tcounter=tcounter>>1;
             }
             else
             {
                tcounter=tcounter*3+1;
             }
             //if(tcounter<MAX)
            //   flagMap[tcounter]=1;
           //  printf("%d ",tcounter);
             tmax++;
            }
            flagMap[counter]=tmax;
           // max=tmax>max?tmax:max;
          }
          flagMap[counter]>max?max=flagMap[counter]:0;
        }
        printf("%d\n",max);
       // for (counter=0;counter<MAX;flagMap[counter++]=0) ;
    }
    return 0;
}



