/*交换函数 2010-09-17*/
#ifndef SWAP_H
#define SWAP_H
int swap(int * a,int * b)
{
    *a=*a^*b;
    *b=*a^*b;
    *a=*a^*b;
    return 0;
}
#endif
