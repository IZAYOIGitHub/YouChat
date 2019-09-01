#include "cJSON.h"

#include<stdio.h>
#include<sys/stat.h>
#include<dirent.h>
#include<assert.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<pthread.h>
#include<netinet/in.h>
#include<time.h>
#include<sys/time.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>




#define LOCAL_PORT 23333
#define SERVER_PORT 23334
#define BUFFER_SIZE 1024




void udp_message_send(int fd, struct sockaddr* destination, char* remote_account){
    
    socklen_t length = sizeof(*destination);
    struct sockaddr_in source;
    char message[BUFFER_SIZE];

    while(1){
        scanf("%s",message);
        cJSON* json =  cJSON_CreateObject();
        cJSON_AddStringToObject(json, "account", remote_account);
        cJSON_AddStringToObject(json, "message", message);
        char* buffer = cJSON_Print(json);
        sendto(fd, buffer, BUFFER_SIZE, 0, destination, length);
        memset(buffer, 0, BUFFER_SIZE);
        cJSON_Delete(json);
    }

}
void* send_message_thread_function(void*  remote_account){

    int local_socket_fd;
    struct sockaddr_in server_address;

    local_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(local_socket_fd < 0){
        printf("create socket fail!\n");
        return -1;
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(SERVER_PORT); 

    udp_message_send(local_socket_fd, (struct sockaddr*)&server_address,  (char*)remote_account);

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



one_to_one_communicate_interface(char* remote_account)
{
    pthread_t send_message_thread;
    pthread_create(&send_message_thread, NULL, send_message_thread_function, remote_account);
    pthread_t receive_message_thread;
    pthread_create(&receive_message_thread, NULL, receive_message_thread_function, NULL);

    int send_message_thread_return, receive_message_thread_return;
    pthread_join(send_message_thread, &send_message_thread_return);
    pthread_join(receive_message_thread, &receive_message_thread_return);
    return 0;
}