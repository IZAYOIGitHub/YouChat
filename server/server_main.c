#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#define MAXLINE 4096

int main(int argc, char** argv){
    int  listenfd, connfd;
    struct sockaddr_in  servaddr;
    char  buff[4096];   //the buffer storing data from client
    int  n;

    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){   //create server's socket using ipv4 & TCP
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    memset(&servaddr, 0, sizeof(servaddr)); //initialize the sockaddr_in
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(6666);    //listenning port:6666

    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){   //bind socket to specific ipaddress and port
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    if( listen(listenfd, 10) == -1){    // listen for client's response
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    printf("======waiting for client's request======\n");
    while(1){
        if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){   //accept data
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            continue;
        }
        n = recv(connfd, buff, MAXLINE, 0);
        buff[n] = '\0';
        printf("recv msg from client:\n %s\n", buff);
        close(connfd);
    }
    close(listenfd);
    return 0;
}