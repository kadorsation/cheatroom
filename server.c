#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>

#define default_name "anonymous";

struct user
{
    char *user_name;
    struct sockaddr_in clientInfo;
    int flag;
};

int main(int argc, char *argv[])
{
   
    fd_set all;
    fd_set select_fd;
    FD_ZERO(&all);
    FD_ZERO(&select_fd);
    int max, maxi = -1;
    int i;
    ssize_t n; 
    int sockfd;
    int listener;
    int nready;
    listener = socket(AF_INET , SOCK_STREAM , 0);
    int newfd;
    socklen_t len;
    char buffer[1025];
    int port = atoi (argv[1]);
    struct sockaddr_in serverInfo,clientInfo;
    bzero(&serverInfo,sizeof(serverInfo));
    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(port);
    bind(listener,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    listen(listener,10);
    FD_SET(listener, &all);
    max = listener;
    struct user client[10];
    for(i = 0; i < 10; i++){
        client[i].flag = -1;
        client[i].user_name = default_name;
    }


    while(1){
        select_fd = all;
        nready = select(max+1,&select_fd,NULL,NULL,NULL);
        if (FD_ISSET(listener, &select_fd)) {    //new connection
            printf("New client!\n");
            
            for (i = 0; i < 10; i++){ 
                if (client[i].flag < 0) { 
                    len = sizeof(client[i].clientInfo);
                    newfd = accept(listener, (struct sockaddr *) &client[i].clientInfo, &len);
                    client[i].flag = newfd;
                    FD_SET(client[i].flag, &all);
                    break; 
                } 
            }
            if (i >= 10){ 
                continue;
            }
            if (newfd > max){ 
                max = newfd;        /* for select */ 
            }
            if (--nready <= 0){ 
                continue;
            }
        }
        for( i = 0; i < 10; i++){
            sockfd = client[i].flag;
            if(sockfd < 0){
                continue;
            }
            if (FD_ISSET(sockfd, &select_fd)){  //因為第81行所以sockfd現在是此cliend的socket值
                printf("%d %d\n", client[i].flag, i);
                memset( buffer, '\0', sizeof( buffer ) );
                n = read(sockfd, buffer, 1025);
                if(n == 0 || buffer == "exit\n"){
                    close(sockfd); 
                    FD_CLR(sockfd, &all); 
                    client[i].flag = -1; 
                }
                else {
                    printf("%s", buffer);
                    write(sockfd, buffer, n); 
                }
                if (--nready <= 0) {
                    break; 
                }
            }
        }
    }
    return 0;
}