#include<stdio.h>
void die_on_error(void) __attribute__((noreturn));

void die_on_error(void)
{
    exit(1);
}
int main (int argc, char const* argv[])
{
    float t3 __attribute__((unused));
    int te=10;
    die_on_error();
    return 0;
}

