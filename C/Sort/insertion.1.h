/* ²åÈëÅÅĞò·¨º¯Êı 2010-03-14*/
int insertSort(int number,int num[])
{
    int counter,comPointer,temp;
    for(counter=1;counter<number;counter++)
    {
        temp=num[counter];
        comPointer=counter-1;
        //for(comPointer=counter-1;comPointer>=0;comPointer--)
        while(comPointer>0&&num[comPointer]>temp)
        {
            if(num[comPointer]>temp)
            {
                num[comPointer+1]=num[comPointer];
            }
            else
            {
                num[comPointer+1]=temp;
                break;
            }
        }
    }
    return 0;
}
