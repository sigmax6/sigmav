#include<stdio.h>
#include<math.h>
int main (int argc, char const* argv[])
{
    double i=10.0;
    double y=0.5;
    double test=pow(i,y);
    printf("%lf\n",test);
    double j=0.01;
    printf("%d\n",abs(y-i));
    if(y>j)
        printf("it works\n");
    return 0;
}
