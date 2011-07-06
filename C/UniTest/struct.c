#include <stdio.h>

struct innerstruct{
    char * name;
    int value;
};
struct segment{
    struct innerstruct start;
    struct innerstruct end;
} ;

struct segment s={{"start",1},{"end",0}};

int main (int argc, char const* argv[])
{
    printf("%s,%d\n",s.start.name,s.start.value);
    return 0;
}

