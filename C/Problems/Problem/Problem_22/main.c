#include<stdio.h>
#define SMAX 10000
#define Level 3
int main(void)
{
    int stuNum,maxNum,compNum,temp,levelCounter,stuCounter,findFlag;
    int stuScore[SMAX];
    while(scanf("%d",&stuNum)&&stuNum!=0)
    {
        levelCounter=0;
        findFlag=0;
        for(stuCounter=0;stuCounter<stuNum;stuCounter++)
        {
            scanf("%d",&stuScore[stuCounter]);
        }
        for(stuCounter=0;stuCounter<stuNum;stuCounter++)
        {
            maxNum=stuCounter;
            compNum=stuCounter+1;
            for (;compNum<stuNum;compNum++)
            {
                if(stuScore[compNum]>=stuScore[maxNum])
                {
                    maxNum=compNum;
                }
            }
            if (maxNum!=stuCounter)
            {
                temp=stuScore[maxNum];
                stuScore[maxNum]=stuScore[stuCounter];
                stuScore[stuCounter]=temp;

            }
            if (stuCounter!=0&&(stuScore[stuCounter]!=stuScore[stuCounter-1]))
            {
                levelCounter++;
            }
            if (levelCounter==Level-1)
            {
                printf("%d\n",stuScore[stuCounter]);
                findFlag=1;
                break;
            }
        }
        if (!findFlag)
           printf("No such score !\n");
    }
    return 0;
}
