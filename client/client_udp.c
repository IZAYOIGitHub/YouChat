#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 2333
#define BUFFER_SIZE 1024


void udp_message_send(int fd, struct sockaddr* destination){
    
    socklen_t length = sizeof(*destination);
    struct sockaddr_in source;
    char buffer[BUFFER_SIZE];

    while(1){
        printf("client:");
        scanf("%s", buffer);
        sendto(fd, buffer, BUFFER_SIZE, 0, destination, length);
        memset(buffer, 0, BUFFER_SIZE);
        recvfrom(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&source, &length);
        printf("%s\n\n",buffer);
    }
}


int main(int argc, char* argv[]){

    int client_socket_fd;
    struct sockaddr_in server_address;
    
    if(argc != 2){
        puts("usage: ./client <destination's IP address>");
        return 0;
    }

    client_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_socket_fd < 0){
        printf("create socket fail!\n");
        return -1;
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    //server_address.sin_addr.s_addr = htonl(INADDR_ANY); 
    server_address.sin_port = htons(SERVER_PORT); 

    udp_message_send(client_socket_fd, (struct sockaddr*)&server_address);

    close(client_socket_fd);
    return 0;
}