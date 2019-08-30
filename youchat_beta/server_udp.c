#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 2333
#define BUFFER_SIZE 1024

void udp_message_receive(int fd){

    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_address;
    socklen_t length = sizeof(client_address);
    
    while(1){
        memset(buffer, 0, BUFFER_SIZE);
        if((recvfrom(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_address, &length)) == -1){
            printf("recieve data fail!\n");
            return 0;
        }
        printf("client:%s\n",buffer);
        memset(buffer, 0, BUFFER_SIZE);
        sprintf(buffer, "The server has recieved your message!\n");
        sendto(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_address, length);
    }
}



int main(int argc, char* argv[]){

    int lserver_socket_fd;
    struct sockaddr_in ser_addr; 

    lserver_socket_fd = socket(AF_INET, SOCK_DGRAM, 0); 
    if(lserver_socket_fd < 0){
        printf("create socket fail!\n");
        return -1;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons(SERVER_PORT); 

    if((bind(lserver_socket_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr))) < 0){
        printf("socket bind fail!\n");
        return -1;
    }

    udp_message_receive(lserver_socket_fd);

    close(lserver_socket_fd);
    return 0;
}