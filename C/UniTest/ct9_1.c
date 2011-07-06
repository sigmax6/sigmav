#include <stdio.h>
#include <limits.h>

/*static int level = 0;*/
int NumConvert(int n, int k) {

    /*level ++;*/
    unsigned int t;

    if (n < 0) {
        t = (unsigned int)((-(n+1))+1);
        putchar('-');
    }
    else if (n > 0)
        t = (unsigned int)n;
    else
        return 0;


    if (t/k != 0)
        NumConvert(t/k, k);

    if(t%k < 10)
        printf("%d",t%k);
    else
        putchar('A'+(t%k-10));

    /*level--;*/

    /*if(level == 0)*/
        /*putchar('\n');*/

    return 0;
}

int NumC2(int n,int k )
{
    int i;
    unsigned t;
    int a[33];
    if( n < 0 ) {
        putchar('-');
        n ++;
        n = -n;
        t = (unsigned)n;
        t ++;
    }else {
        t = (unsigned)n;
    }
    for( i = 0; t && i < 33; i++ ) {
        a[i] = t % k;
        t /= k;
    }
    for( i--; i >= 0; i-- ) {
        if( a[i] >= 0 && a[i] <= 9 ) {
            printf("%d",a[i]);
        }else {
            putchar(a[i]-10+'A');
        }
    }
    return 0;
}

int main (int argc, char const* argv[])
{

    NumConvert(-1,2);
    NumConvert(0,2);
    NumConvert(2,2);
    NumConvert(9,4);
    NumConvert(31,16);
    NumConvert(-31,16);
    NumConvert(-100,16);
    NumConvert(INT_MAX,16);
    NumConvert(-INT_MAX,16);
    puts("");

    NumC2(-1,2);
    NumC2(0,2);
    NumC2(2,2);
    NumC2(9,4);
    NumC2(31,16);
    NumC2(-31,16);
    NumC2(-100,16);
    NumC2(INT_MAX,16);
    NumC2(-INT_MAX,16);

    return 0;

}

