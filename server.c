#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc , char *argv[])

{
    //socket的建立
    char inputBuffer[256] = {};
    char message[] = {"Hi,this is server.\n"};
    int sockfd = 0,forClientSockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);

    if (sockfd == -1){
        printf("Fail to create a socket.");
    }

    //socket的連線
    struct sockaddr_in serverInfo,clientInfo;
    int addrlen = sizeof(clientInfo);
    bzero(&serverInfo,sizeof(serverInfo));
    int port = atoi (argv[1]);
    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;

    serverInfo.sin_port = htons(port);
    bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    listen(sockfd,5);

    while(1){
        forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
        while(inputBuffer!="close"){
            char inputBuffer[256] = {};
            read(forClientSockfd,inputBuffer,sizeof(inputBuffer));
            printf("Get:%s\n",inputBuffer);
            write(forClientSockfd,inputBuffer,sizeof(inputBuffer));
        }
    }
    return 0;
}