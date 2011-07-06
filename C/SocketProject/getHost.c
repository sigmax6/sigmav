#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

/*struct hostent * host*/
int main (int argc, char const* argv[])
{
    char * ip = argv[1];
    in_addr_t data = inet_addr(ip);
    struct hostent * gethost = gethostbyaddr(&data,4,AF_INET);
    if (gethost == NULL)
    {
        printf("Unknown host\n");
        exit(1);
    }
    printf("host name :%s \n ",gethost->h_name);
    return 0;
}

