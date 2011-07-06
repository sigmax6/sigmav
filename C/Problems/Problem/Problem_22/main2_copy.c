/*#include <stdio.h>
#include <conio.h>
main( )
{
      long a,n,max,target,second;
     while(scanf("%d",&n)&&n>0)
      { max=second=target=-1;
        for(;n>0;n--)
        { scanf("%d",&a);
          if((a^max&&a^second&&a^target))
          {
            if(a>max)  {target=second;second=max; max=a;}
            else if(a>second) {target=second;second=a;}
            else if(a>target)  target=a;
          }
        }
        if(target!=-1)
        printf("%ld\n",target);
        else
        printf("No such score !\n");
      }
      getch( );
      return 0;
}
*/
#include<stdio.h>
int main(void)
{
    int inputNum,input,first,second,third;
    while(scanf("%d",&inputNum)&&inputNum!=0)
    {
        first=second=third=-1;
        for(;inputNum>0;inputNum--)
        {
            scanf("%d",&input);
            if(input!=first&&input!=second&&input!=third)
            {
                if(input>first)
                {
                    third=second;
                    second=first;
                    first=input;
                }
                else if(input>second)
                {
                    third=second;
                    second=input;
                }
                else if(input>third)
                    third=input;
            }
        }
        if (third!=-1)
           printf("%d\n",third);
        else
           printf("No such score !\n");
    }
    return 0;
}
