#include <stdio.h>
#include <stdlib.h>
int course[1000] = {0};
int main( void )
{
        int n, k, i, j, tmp, sum;
        while( EOF != scanf( "%d%d", &n, &k ) )
        {
                for( i = 0; i < n - 1; ++i )
                {
                        for( j = 0, sum = 0; j < k; ++j )
                        {
                                scanf( "%d", &tmp );
                sum += tmp;
                course[j] += tmp;
                        }
            printf( "%d ", sum );
                }
        for ( j = 0, sum = 0; j < k; ++j )
        {
            scanf( "%d", &tmp );
            sum += tmp;
            course[j] += tmp;
        }
        printf( "%d\n", sum );
        tmp = n / 2;
        for( j = 0;j < k - 1; ++j )
        {
            printf( "%d ", ( course[j] + tmp ) / n );
            course[j] = 0;
        }
        printf( "%d\n\n", ( course[j] + tmp ) / n );
        course[j] = 0;
        }
        return 0;
}
