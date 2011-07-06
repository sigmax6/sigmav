#include <signal.h>
#include <stdio.h>
#include <termios.h>

#define MAX_PASS_LEN 8               //max chars for user to enter

char * getpass(const char * prompt)  //get any password for the setted prompt
{
    static char buf[MAX_PASS_LEN+1]; //null byte at end;
    FILE * fp;
    sigset_t sig,sigsave;            //signal control sig-> sigsave
    struct termios term,termsave;
    char * ptr;
    int c;

    if((fp=fopen(ctermid(NULL),"r+"))==NULL) //open the default device for i/o
    {
        return(NULL);
    }

    setbuf(fp,NULL);                 //Set to no buff mode

    sigemptyset(&sig);
    sigaddset(&sig,SIGINT);
    sigaddset(&sig,SIGTSTP);
    sigprocmask(SIG_BLOCK,&sig,&sigsave);    //Mask the SIGINT SIGTSTP signal and save to sigsave

    tcgetattr(fileno(fp),&termsave);       //save the tty state
    term=termsave;
    term.c_cflag &=~(ECHO|ECHOE|ECHOK|ECHONL); //trun off the echo
    tcsetattr(fileno(fp),TCSAFLUSH,&term);

    fputs(prompt,fp);          //print the prompt message;

    ptr=buf;

    while((c=getc(fp))!=EOF && c!='\n')
    {
        if(ptr<&buf[MAX_PASS_LEN])
        {
            *ptr++=c;
        }
    }
    *ptr=0;         //Null terminalte
    putc('\n',fp);  //echo a new line

    tcsetattr(fileno(fp),TCSAFLUSH,&termsave);
    sigpromask(SIG_SETMASK,&sigsave,NULL);

    fclose(fp);

    return(buf);

}

