#include<stdio.h>
#include<time.h>
#define GET_SIZE  (sizeof(time_t)<sizeof(double)?sizeof(double):sizeof(time_t))
union test
{
    time_t test_time_t;
    double test_double;
    unsigned char test_char[GET_SIZE];
};

int main (int argc, char const* argv[])
{
    union test TEST;
    int i;
    memset(&TEST,1,sizeof(union test));
    TEST.test_time_t=9;
    for(i=GET_SIZE-1;i>=0;i--)
    {
        printf("%02x ",TEST.test_char[i]);
    }
    putchar('\n');
    return 0;
}
