#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LOCAL_PORT 23333
#define REMOTE_PORT 23333
#define BUFFER_SIZE 1024



void* send_message_udp(char* buffer){

    int local_socket_fd;
    struct sockaddr_in remote_info;

    local_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(local_socket_fd < 0){
        printf("create socket fail!\n");
        return;
    }

    char destination_IP[15] = {'\0'};
    puts("Input destination's IP:");
    scanf("%s", destination_IP);

    memset(&remote_info, 0, sizeof(remote_info));
    remote_info.sin_family = AF_INET;
    remote_info.sin_addr.s_addr = inet_addr(destination_IP);
    remote_info.sin_port = htons(REMOTE_PORT); 

    struct sockaddr* destination = (struct sockaddr*)&remote_info;
    socklen_t length = sizeof(*destination);
    sendto(local_socket_fd, buffer, BUFFER_SIZE, 0, destination, length);

    close(local_socket_fd);
    return 0;

}



void receive_message_and_send(){

    int local_socket_fd;
    struct sockaddr_in local_info; 

    local_socket_fd = socket(AF_INET, SOCK_DGRAM, 0); 
    if(local_socket_fd < 0){
        printf("create socket fail!\n");
        return -1;
    }

    memset(&local_info, 0, sizeof(local_info));
    local_info.sin_family = AF_INET;
    local_info.sin_addr.s_addr = htonl(INADDR_ANY);
    local_info.sin_port = htons(LOCAL_PORT); 

    if((bind(local_socket_fd, (struct sockaddr*)&local_info, sizeof(local_info))) < 0){
        printf("socket bind fail!\n");
        return -1;
    }

    char buffer[BUFFER_SIZE];
    struct sockaddr_in remote_info;
    socklen_t length = sizeof(remote_info);
    
    while(1){
        memset(buffer, 0, BUFFER_SIZE);
        if((recvfrom(local_socket_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&remote_info, &length)) == -1){
            printf("recieve data fail!\n");
            return;
        }
        printf("transmit: %s\n",buffer);
        send_message_udp(buffer);
    }

    close(local_socket_fd);
    return 0;

}



int main(int argc, char* argv[]){

    puts("\n\n>========== YouChat version beta 1 is running exhaustively! ==========<\n\n");

    receive_message_and_send();

    return 0;

}
