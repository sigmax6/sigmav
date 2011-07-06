/* ²åÈëÅÅĞò·¨º¯Êı 2010-03-14*/
int insertSort(int number,int num[])
{
    int counter,comPointer,temp;
    for(counter=1;counter<number;counter++)
    {
        temp=num[counter];
        for(comPointer=counter-1;comPointer>=0;comPointer--)
        {
            if(num[comPointer]>num[counter])
            {
                num[comPointer+1]=num[comPointer];
            }
            else
            {
                
            }
        }
    }
}
