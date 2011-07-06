//FindPatKMP.h KMP字符搜索函数定义
#include <string>
#include <assert.h>
using namespace std;
//FindNext 函数 输入字符串S 返回指向S特征向量的int指针N
void FindNext(string S,int []N)
{
    int strSize=S.size();                        //字符串长度
    for(int i=0;i<strSize;i++)
    {
        if(0==i)                          //第0个元素
        {
            N[i]=0;                       //特征向量为0
        }
        else
        {
            int k=N[i-1];                     //已知前一特征向量为k
            while(k>0&&S[i]!=S[k])
            {
                k=N[k-1];                 //当当前失配时，向前一个匹配串回溯
            }
            if(S[i]==S[k])                //找到匹配串
            {
                N[i]=k+1;                 //设置当前串特征向量
            }
            else
            {
                N[i]=0;                   //若找不到匹配，设置当前特征值0
            }
        }
    }
}
//FindPatKMP函数 输入匹配目标串S 匹配串P 开始匹配位置startIndex
int FindPatKMP(string S,string P,int startIndex)
{
    int lastIndex=S.size()-P.size();      //计算能匹配的最后位置
    if(lastIndex<startIndex)
    {
        return -1;                        //若开始位置在最后位置后 失败
    }
    else
    {
       int j=0;
       int * N=new int[strSize];                    //为特征向量申请strSize长的空间
       N=FindNext(P);                //获取S串的特征向量 保存于N中
       for(int i=startIndex;i<S.size();i++)
       {
           while(S[i]!=P[j]&&j>0)         //若当前位置到不到匹配，
           {
               j=N[j-1];                  //回溯匹配
           }
           if(S[i]==P[j])
           {
               j++;                       //找到匹配位置后游标移动
           }
           if(j==P.size())                //若j游标达到了P结尾，匹配成功
           {
              // delete []N;
               return i-j+1;              //返回匹配位置
           }
       }
       return -1;
    }
}
