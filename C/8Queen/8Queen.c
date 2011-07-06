//----------------------------------------------------------------------------
//Sat Apr 24 14:45:44 CST 2010
//八皇后问题 回溯法 sigmax6
// SVN 测试
//---------------------------------------------------------------------------
#include<stdio.h>
#define QUEENS 10                            //定义皇后问题规模
//------------------------------------------------------------------------------
    static char Queen[QUEENS][QUEENS];       //用于存放摆放位置信息的二维数组
    static int yQueen[QUEENS];               //存放每行皇后信息  1  无  0  有
    static int oxyQueen[QUEENS*2-1];         //左上至右下皇后信息1  无  0  有
    static int xyQueen[QUEENS*2-1];          //右上至左下皇后信息1  无  0  有
    static int sul=0;                        //解决方案计数器
//------------------------------------------------------------------------------
int initq()
{
    int i,j;
    for (i=0;i<QUEENS;i++)
    {
        yQueen[i]=1;
    }
    for (j=0;j<QUEENS*2-1;j++)
    {
        oxyQueen[j]=xyQueen[j]=1;
    }
    for(i=0;i<QUEENS;i++)
    {
        for(j=0;j<QUEENS;j++)
        {
            Queen[i][j]='*';
        }
    }
    return 0;
}
int printq()
{
    int i,j;
    printf("//-----------------------------------------------------------------------------------//\n");
    printf("//--------------------------------SUL:%d---------------------------------------------//\n",sul);
    for(i=0;i<QUEENS;i++)
    {
        printf("                       \n");
        for(j=0;j<QUEENS;j++)
        {
            printf("%3c",Queen[i][j]);
        }
        printf("\n");
    }
    return 0;
}
int solute(int Qx)
{                                                           //递归解题函数
   int Qy=0;                                                //标定行数
   for(;Qy<QUEENS;Qy++)
   {
      if(yQueen[Qy]&&oxyQueen[Qx-Qy+7]&&xyQueen[Qx+Qy])     //判断是否可以存放
      {
          yQueen[Qy]=0;
          oxyQueen[Qx-Qy+7]=0;
          xyQueen[Qx+Qy]=0;
          Queen[Qx][Qy]='@';
          if(Qx<QUEENS-1)
          {
              solute(Qx+1);
          }
          else
          {
              sul++;
              printq();
          }
          //本次输入结束清除标记信息
         yQueen[Qy]=1;
         oxyQueen[Qx-Qy+7]=1;
         xyQueen[Qx+Qy]=1;
         Queen[Qx][Qy]='*';
         //------------------------
      }
   }
   return 0;
}
int main(int argc,char * * argv)
{
    int Qx=0;                         //标定皇后 共八个 ：0 1 2 3 4 5 6 7 对应棋盘八列
    initq();                          //初始化函数
    solute(Qx);                       //进入递归解题
    return 0;
}


