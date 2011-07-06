/*   套接字使用测试程序 _ 客户端 */

//需要添加的头文件
#include<sys/types.h>
#include<sys/socket.h>
//----------------
#include<stdio.h>
#include<sys/un.h>
#include<unistd.h>

int main (int argc, char const* argv[])
{
    int sockfd;              //
    int len;
    struct sockaddr_un address;
    int result;
    char ch='A';
    sockfd=socket(AF_UNIX,SOCK_STREAM,0);
    address.sun_family=AF_UNIX;
    strcpy(address.sun_path,"server_socket");
    len=sizeof(address);
    result=connect(sockfd,(struct sockaddr *)&address,len);
    if(result==-1)
    {
        perror("client1");
        exit(1);
    }
    write(sockfd,&ch,1);
    read(sockfd,&ch,1);
    printf("char form server =%c\n",ch);
    close(sockfd);
    exit(0);
}
