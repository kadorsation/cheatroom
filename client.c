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
    int sockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);

    if (sockfd == -1){
        printf("Fail to create a socket.");
    }

    //socket的連線

    struct sockaddr_in info;
    bzero(&info,sizeof(info));
    info.sin_family = PF_INET;

    //localhost test
    int port = atoi (argv[2]);
    info.sin_addr.s_addr = inet_addr(argv[1]);
    info.sin_port = htons(port);


    int err = connect(sockfd,(struct sockaddr *)&info,sizeof(info));
    if(err==-1){
        printf("Connection error");
    }


    //Send a message to server

    char message[] = {"Hi there"};
    while(message!="close"){
        scanf("%s",message);
        char receiveMessage[256] = {};
        write(sockfd,message,sizeof(message));
        read(sockfd,receiveMessage,sizeof(receiveMessage));
        printf("receive:%s\n",receiveMessage);
    }
    printf("close Socket\n");
    close(sockfd);
    return 0;
}
