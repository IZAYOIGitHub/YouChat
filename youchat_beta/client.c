#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LOCAL_PORT 23333
#define REMOTE_PORT 23333
#define BUFFER_SIZE 1024




void udp_message_send(int fd, struct sockaddr* destination){
    
    socklen_t length = sizeof(*destination);
    struct sockaddr_in source;
    char buffer[BUFFER_SIZE];

    while(1){
        scanf("%s", buffer);
        sendto(fd, buffer, BUFFER_SIZE, 0, destination, length);
        memset(buffer, 0, BUFFER_SIZE);
    }

}
void* send_message_thread_function(void* destination_IP){

    int local_socket_fd;
    struct sockaddr_in remote_address;

    local_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(local_socket_fd < 0){
        printf("create socket fail!\n");
        return -1;
    }

    memset(&remote_address, 0, sizeof(remote_address));
    remote_address.sin_family = AF_INET;
    remote_address.sin_addr.s_addr = inet_addr((char*)destination_IP);
    remote_address.sin_port = htons(REMOTE_PORT); 

    udp_message_send(local_socket_fd, (struct sockaddr*)&remote_address);

    close(local_socket_fd);
    return 0;

}



void udp_message_receive(int fd){

    char buffer[BUFFER_SIZE];
    struct sockaddr_in remote_address;
    socklen_t length = sizeof(remote_address);
    
    while(1){
        memset(buffer, 0, BUFFER_SIZE);
        if((recvfrom(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&remote_address, &length)) == -1){
            printf("recieve data fail!\n");
            return 0;
        }
        printf("%s\n",buffer);
    }

}
void* receive_message_thread_function(){

    int local_socket_fd;
    struct sockaddr_in local_address; 

    local_socket_fd = socket(AF_INET, SOCK_DGRAM, 0); 
    if(local_socket_fd < 0){
        printf("create socket fail!\n");
        return -1;
    }

    memset(&local_address, 0, sizeof(local_address));
    local_address.sin_family = AF_INET;
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);
    local_address.sin_port = htons(LOCAL_PORT); 

    if((bind(local_socket_fd, (struct sockaddr*)&local_address, sizeof(local_address))) < 0){
        printf("socket bind fail!\n");
        return -1;
    }

    udp_message_receive(local_socket_fd);

    close(local_socket_fd);
    return 0;

}



int main(int argc, char* argv[]){

    char IP[15] = {'\0'};
    puts("\n\n>========== YouChat version beta 1 is running exhaustively! ==========<\n\n");
	puts("Input destination IP:");
	scanf("%s", IP);

    pthread_t send_message_thread;
    pthread_create(&send_message_thread, NULL, send_message_thread_function, (void*)&IP);
    pthread_t receive_message_thread;
    pthread_create(&receive_message_thread, NULL, receive_message_thread_function, NULL);

    int send_message_thread_return, receive_message_thread_return;
    pthread_join(send_message_thread, &send_message_thread_return);
    pthread_join(receive_message_thread, &receive_message_thread_return);

    return 0;

}
