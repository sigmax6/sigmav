#include <stdio.h>
#include "myheap.h"

#define LENGTH 10
int main (int argc, char const* argv[])
{
    int A[LENGTH];
    int counter;
    for (counter = 0 ; counter < LENGTH ;counter ++)
    {
      //  scanf("%d",&A[counter]);
        A[counter]= LENGTH - counter;
    }

    // Sort func here
    heapSort(A,LENGTH);

    for (counter = 0; counter < LENGTH ; counter ++)
    {
        printf ("%d ",A[counter]);
    }

    return 0;
}

