#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>

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
    int client[10];
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
    for(i = 0 ; i < 10 ; i++){
        client[i] = -1;
    }
    while(1){
        select_fd = all;
        nready = select(max+1,&select_fd,NULL,NULL,NULL);
        if (FD_ISSET(listener, &select_fd)) {    //new connection
            printf("New client!\n");
            len = sizeof(clientInfo);
            newfd =  accept(listener, (struct sockaddr *) &clientInfo, &len);
            
            for (i = 0; i < 10; i++){ 
                if (client[i] < 0) { 
                    client[i] = newfd;
                    break; 
                } 
            }
            if (i >= 10){ 
                continue;
            }
            FD_SET(newfd, &all);
            if (newfd > max){ 
                max = newfd;        /* for select */ 
            }
            if (i > maxi){ 
                maxi = i;       /* max index in client[] array */ 
            }
            if (--nready <= 0){ 
                continue;
            }
        }
        for( i = 0; i <= maxi; i++){
            sockfd = client[i];
            if(sockfd < 0){
                continue;
            }
            if (FD_ISSET(sockfd, &select_fd)){
                memset( buffer, '\0', sizeof( buffer ) );
                n = read(sockfd, buffer, 1025);
                printf("%s",buffer);
                if(n == 0){
                    close(sockfd); 
                    FD_CLR(sockfd, &all); 
                    client[i] = -1; 
                }
                else {
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