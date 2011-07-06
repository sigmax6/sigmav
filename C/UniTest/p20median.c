#include <stdio.h>
#include <stdlib.h>

void swap(int * a,int * b)
{
    int temp=*a;
    *a=*b;
    *b=temp;
}

int mdiv(int * number,int sptr,int lptr)
{
    int med=number[lptr];
    int j=sptr;
    for(;sptr<lptr;sptr++)
    {
        if(number[sptr]<med)
        {
            if(sptr!=j)
            {
                swap(&number[sptr],&number[j]);
            }
            j++;
        }
    }
    swap(&number[j],&number[lptr]);
    return j;
}

void mqsort(int * number,int sptr,int lptr,int mid )
{
    if(lptr>sptr)
    {
       int mptr = mdiv(number,sptr,lptr);
       if(mptr>mid)
       {
           return mqsort(number,sptr,mptr-1,mid);
       }
       else if(mptr<mid)
       {
           return mqsort(number,mptr+1,lptr,mid);
       }
    }
}
/*int compar(const void * a,const void * b)*/
/*{*/
    /*return *((int *)a)-*((int *)b);*/
/*}*/

int main(int argc, char const* argv[])
{
    int total,i;
    int * ptr=NULL;

    /*scanf("%d", &total);*/
    while ((scanf("%d", &total) != EOF) && ((total & 1) != 0)) {

        ptr = (int *) malloc(total * sizeof(int));
        for (i=0; i<total; i++)
            scanf("%d", ptr+i);

        mqsort(ptr,0,total-1,total/2);
        /*qsort(ptr,total,sizeof(int),compar);*/
        printf("%d\n",ptr[total/2]);

        free(ptr);
    }

    return 0;
}
