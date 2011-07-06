#include<stdio.h>
int main(void)
{
    int counter;
    int input;
    int pflag;
//    int dcounter;
  while(scanf("%d",&input)!=EOF)
  {
    pflag=0;
    printf("%d = ",input);
    for(counter=2;counter<=input;)
    {
        if(0==input%counter)
        {
            input=input/counter;
            if(0==pflag)
            {
               printf("%d",counter);
               pflag++;
            }
            else
            {
                pflag++;
            }
        }
        else
        {
            if (pflag>1)
            {
                printf("^%d * ",pflag);
                //pflag=0;
            }
            else if(pflag==1)
            {
                printf(" * ");
            }
            counter++;
            pflag=0;
        }
    }
    if(pflag>1)
    {
        printf("^%d",pflag);
    }
    //printf("%d\n",input);
    putchar('\n');
  }
  return 0;
}

