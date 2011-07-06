int binarySearchIterate(int begin,int end,int number,int const num[])
{
    int middle;
    while(begin!=end)
    {
        middle=(begin+end)/2;
        if(number==num[middle])
        {
            return middle;
        }
        else if(number>num[middle])
        {
            begin=middle+1;
        }
        else
        {
            end=middle;
        }
    }
    return begin;
}
