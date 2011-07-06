#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void sig_catch(int);

int main (int argc, char const* argv[])
{

    if(signal(SIGINT,sig_catch)==SIG_ERR)
    {
        printf("catch SIGINT error!\n");
        exit(-1);
    }

    if (signal(SIGQUIT,sig_catch)==SIG_ERR)
    {
        printf("catch SIGQUIT error!\n");
        exit(-1);
    }

    if(signal(SIGTERM,sig_catch)==SIG_ERR)
    {
        printf("catch SIGTERM error!\n");
        exit(-1);                                     //Catch the signals
    }

    // RAW MODE
    if(tty_raw())

}
