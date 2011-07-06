#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define LEON
//#define HGEN

#define UNLIMIT
//#define PRINT
#define MAXARRAY 60000 /* this number, if too large, will cause a seg. fault!! */

#ifdef LEON
#include "qsort_large.h"
#endif

struct my3DVertexStruct {
  int x, y, z;
  double distance;
};

#ifndef HGEN
int compare(const void *elem1, const void *elem2)
{
  /* D = [(x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2]^(1/2) */
  /* sort based on distances from the origin... */

  double distance1, distance2;

  distance1 = (*((struct my3DVertexStruct *)elem1)).distance;
  distance2 = (*((struct my3DVertexStruct *)elem2)).distance;

  return (distance1 > distance2) ? 1 : ((distance1 == distance2) ? 0 : -1);
}
#endif

int
main(int argc, char *argv[]) {

  struct my3DVertexStruct array[MAXARRAY];

#ifndef LEON
  FILE *fp;
#endif

#ifdef HGEN
  FILE *fp_out;
#endif

#ifdef PRINT
  int i;
#endif

  int count=0;
  int x, y, z;

#ifndef LEON
  if (argc<2) {
    fprintf(stderr,"Usage: qsort_large <file>\n");
    exit(-1);
  }
  else {
    fp = fopen(argv[1],"r");

    while((fscanf(fp, "%d", &x) == 1) && (fscanf(fp, "%d", &y) == 1) && (fscanf(fp, "%d", &z) == 1) &&  (count < MAXARRAY)) {
	 array[count].x = x;
	 array[count].y = y;
	 array[count].z = z;
	 array[count].distance = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	 count++;
    }
  }
#endif

#ifdef HGEN
  fp_out = fopen("qsort_large.h","w");

  fprintf(fp_out,"static int qsort_large_array[%d][3]={\n",MAXARRAY);
  for(count=0;count < MAXARRAY ;count++)
  {
      if(count != 0)
      {
          fprintf(fp_out,",\n");
      }
      fprintf(fp_out,"{%d,%d,%d}",array[count].x,array[count].y,array[count].z);
  }
  fprintf(fp_out , "\n};\n");
#endif

#ifdef LEON
  for(count=0 ; count < MAXARRAY ; count++)
  {
	 x = qsort_large_array[count][0];
	 y = qsort_large_array[count][1];
	 z = qsort_large_array[count][2];
     array[count].x = x;
	 array[count].y = y;
	 array[count].z = z;
     array[count].distance = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
  }
#endif

#ifndef HGEN
  printf("\nSorting %d vectors based on distance from the origin.\n\n",count);
  clock_t time= clock();
  qsort(array,count,sizeof(struct my3DVertexStruct),compare);
#endif

#ifdef PRINT
  for(i=0;i<count;i++)
    printf("%d %d %d\n", array[i].x, array[i].y, array[i].z);
#endif

  time = clock()-time;
  double duration = (double)time/CLOCKS_PER_SEC;
  printf("Sorted!\n Time used : %f secs.\n",duration);


  return 0;
}
