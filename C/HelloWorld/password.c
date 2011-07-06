#include<stdio.h>

#define WMAX 20
#defind WMAXL 10
int main(void)
{
    int wordNum;
    int wordCount;
    int passLength;
    char words[WMAX][WMAXL];
    printf("请输入单词数：\n");
    scanf("%d",&wordNum);
    wordCount=wordNum;
    printf("请输入生成密码所用单词：\n");
    for(;wordNum!=0;wordNum--)
    {
        printf("剩余%d个:",wordNum);
        scanf("%s",words[wordNum]);
    }
    printf("请输入所需密码的最大长度：")
    scnanf("%d",&passLength);

}
