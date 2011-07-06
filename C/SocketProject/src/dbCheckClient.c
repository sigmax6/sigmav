//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <stdio.h>        // for printf
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero

#define CHECK_SERVER_PORT  6666
#define BUFFER_SIZE 1024
#define CHECK_FOLDER  "~/Testfiles/"
#define MAX_NAME_LEN 256

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: ./%s ServerIPAddress\n",argv[0]);
        exit(1);
    }

    //Socket Init
    struct sockaddr_in client_addr;
    bzero(&client_addr,sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htons(INADDR_ANY);
    client_addr.sin_port = htons(0);
    int client_socket = socket(AF_INET,SOCK_STREAM,0);
    if( client_socket < 0)
    {
        printf("Create Socket Failed!\n");
        exit(1);
    }
    if( bind(client_socket,(struct sockaddr*)&client_addr,sizeof(client_addr)))
    {
        printf("Client Bind Port Failed!\n");
        exit(1);
    }
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if(inet_aton(argv[1],&server_addr.sin_addr) == 0)
    {
        printf("Server IP Address Error!\n");
        exit(1);
    }
    server_addr.sin_port = htons(CHECK_SERVER_PORT);
    socklen_t server_addr_length = sizeof(server_addr);

    if(connect(client_socket,(struct sockaddr*)&server_addr, server_addr_length) < 0)
    {
        printf("Can Not Connect To %s!\n",argv[1]);
        exit(1);
    }


    //Trans data
    char buffer[BUFFER_SIZE];
    bzero(buffer,BUFFER_SIZE);

    while(1)
    {
        int length = recv(client_socket,buffer,BUFFER_SIZE,0);
        if(length < 0)
        {
            printf("Recieve Data From Server %s Failed!\n", argv[1]);

        }
        if (strcmp(buffer,"Init") != 0)
        {
            printf("server not inited\n");
            break;
        }

        bzero(buffer,BUFFER_SIZE);
        strcpy(buffer,"Check");
        send(client_socket,buffer,BUFFER_SIZE,0);
        printf("Server is Checking data....\n");
        bzero(buffer,BUFFER_SIZE);
        length=recv(client_socket,buffer,BUFFER_SIZE,0);
        if(length<0)
        {
            printf("Recieve Data From Server %s Failed!\n",argv[1]);
            break;
        }
        if(strcmp(buffer,"Error") == 0)
        {
            bzero(buffer,BUFFER_SIZE);
            recv(client_socket,buffer,BUFFER_SIZE,0);
            printf("%s\n",buffer);
            //TODO STUFF HERE -> syslog_ng
        }
        else if(strcmp(buffer,"Succeed") == 0)
        {
            char logName[MAX_NAME_LEN];
            int  logFd;
            bzero(buffer,BUFFER_SIZE);
            recv(client_socket,buffer,BUFFER_SIZE,0);
            strcpy(logName,buffer);
            if( (logFd=open(logName,O_RDWR)) < 0)
            {
                printf("Open file %s error\n",logName);
                break;
            }

            bzero(buffer,BUFFER_SIZE);
            recv(client_socket,buffer,BUFFER_SIZE,0);
            while(strsmp(buffer,"EOF") != 0)
            {
                 fputs(buffer,logFd);
                 bzero(buffer,BUFFER_SIZE);
                 recv(client_socket,buffer,BUFFER_SIZE,0);
            }
            close(logFd);
            //TODO log file stuff here
        }
    }
    //关闭socket
    close(client_socket);
    return 0;
}
