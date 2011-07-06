#ifndef MYHEAP_H
#define MYHEAP_H

#define PARENT(i) ((i-1)/2)
#define LEFT(i) (2*(i)+1)
#define RIGHT(i) (2*(i)+2)

void swap(int *,int *);
void maxHeapify(int [],int,int);
void buildMaxHeap(int [],int);
int  heapSort(int [],int);

#endif
