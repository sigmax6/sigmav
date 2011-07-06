#include <stdio.h>

struct test
{
    int i;
    int j;
};

int uname (struct test * ptr)
{
    printf ("%d",ptr->i);
    return 0;
}
int main (int argc, char const* argv[])
{
    struct test str;
    printf ("%d\n",&str);
    struct test * sptr = & str;
    printf ("%d\n",&str);
    str.i = 1;
    printf ("%d\n",&str);
    uname(sptr);

}

