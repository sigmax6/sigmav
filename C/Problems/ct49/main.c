#include <stdio.h>
#define MAX 10001
#define CMAX 255
int main(void)
{
    unsigned char str1[MAX],str2[MAX];
    int str1Map[CMAX]={0},str2Map[CMAX]={0};
    int A,B,lstr1,lstr2,counter,slength,blength,flag;
    while(scanf("%s%s",str1,str2)!=EOF)
    {
        A=0;
        B=0;
        for(lstr1=0;str1[lstr1]!='\0';lstr1++) ;
        for(lstr2=0;str2[lstr2]!='\0';lstr2++) ;
        slength=lstr1<lstr2?(flag=0,blength=lstr2,lstr1):(flag=1,blength=lstr1,lstr2);
        for (counter=0;counter<slength;counter++)
        {
            if(str1[counter]==str2[counter])
            {
               // A++;
                str1[counter]=str2[counter]='\0';
            }
            str1Map[str1[counter]]++;
            str2Map[str2[counter]]++;
        }
        if(0==flag)
        {
          for (counter=slength;counter<blength;counter++)
          {
            str2Map[str2[counter]]++;
          }
        }
        if(1==flag)
        {
          for (counter=slength;counter<blength;counter++)
          {
            str1Map[str1[counter]]++;
          }
        }
        A=str1Map[0];
        for(counter=1;counter<CMAX;counter++)
        {
            B+=str1Map[counter]<str2Map[counter]?str1Map[counter]:str2Map[counter];
        }
        printf("%dA%dB\n",A,B);
        for(counter=0;counter<MAX;str1[counter]='\0',str2[counter++]='\0') ;
        for(counter=0;counter<CMAX;str1Map[counter]=0,str2Map[counter++]=0) ;
    }
    return 0;
}
