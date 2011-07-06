#include<sys/types.h>
#include<sys/socket.h>
#include<linux/in.h>
#include<linux/net.h>
#define MY_PORT 8964

int main (int argc, char const* argv[])
{
    int sockfd,newfd;
    int cpid;              //child pid;
    struct sockaddr_in servaddr;
    struct sockaddr_in clientInfo;
    //Create socket
    if(sockfd=socket(AF_INET,SOCK_STREAM,0)<0)
    {
        printf("Unable to create socked!\n");
        exit(0);
    }

#ifdef LINUX
    int opt=1;
    int len=sizeof(opt);
    setsockopt(sockfd,SOL_SOCKET,SO_RESUEADDR,&opt,&len);
#endif


    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_family=htons(MY_PORT);

    if(bind(sockfd,(struct sockaddr *)&servaddr,sizeof(struct sockaddr))<0)
    {
        printf("Unable to bind socket!\n");
        exit(0);
    }
    listen(sockfd,5);
    while(1)
    {
        newfd=accept(sockfd,(struct sockaddr *)&clientInfo,sizeof(struct sockaddr));

    }
}

