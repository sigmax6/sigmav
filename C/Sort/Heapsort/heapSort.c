#include <stdio.h>
#include "myheap.h"

#define PARENT(i) ((i-1)/2)
#define LEFT(i) (2*(i)+1)
#define RIGHT(i) (2*(i)+2)

void swap(int * A, int * B)
{
    *A = (*A)^(*B);
    *B = (*A)^(*B);
    *A = (*A)^(*B);
}

void maxHeapify(int A[],int i,int length)
{
    int l=LEFT(i);
    int r=RIGHT(i);
    int largest;

    if(l < length && A[l] > A[i])
    {
        largest = l;
    }
    else
    {
        largest = i;
    }
    if(r < length && A[r] > A[largest])
    {
        largest = r;
    }

    if(largest != i)
    {
        swap(&A[largest],&A[i]);
        maxHeapify(A,largest,length);
    }
}

void buildMaxHeap(int A[],int length)
{
    int counter;
    int heapSize = length/2;
    for(counter=0 ; counter < heapSize  ; counter++)
    {
       maxHeapify(A,counter,length);
    }
}

int heapSort(int A[],int length)
{
    int counter;
    for (counter = length ; counter > 0 ;counter--)
    {
        buildMaxHeap(A,counter);
        swap(&A[0],&A[counter]);
    }

    return 0;
}
//int main (int argc, char const* argv[])
//{
//    int A[LENGTH];
//    int i;
//
//    for (i=0; i< LENGTH ;i++)
//    {
//        A[i]=i;
//    }
//
//    buildMaxHeap(A,LENGTH);
//
//    for (i=0 ; i < LENGTH ; i++)
//    {
//        printf ("%d ,",A[i]);
//    }
//
//    return 0;
//}


