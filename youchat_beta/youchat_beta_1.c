#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LOCAL_PORT 2334
#define REMOTE_PORT 2333
#define BUFFER_SIZE 1024



typedef struct sys_arg_{
    int sys_argc;
    char* sys_argv1;
}ARG;




void udp_message_send(int fd, struct sockaddr* destination){
    
    socklen_t length = sizeof(*destination);
    struct sockaddr_in source;
    char buffer[BUFFER_SIZE];

	sleep(0.1);
    while(1){
        printf("client:");
        scanf("%s", buffer);
        sendto(fd, buffer, BUFFER_SIZE, 0, destination, length);
        memset(buffer, 0, BUFFER_SIZE);
        recvfrom(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&source, &length);
        printf("%s\n\n",buffer);
    }

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
        printf("client:%s\n",buffer);
        memset(buffer, 0, BUFFER_SIZE);
        sprintf(buffer, "The remote has recieved your message!\n");
        sendto(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&remote_address, length);
    }

}



void* send_message_thread_function(void *arg_){

    ARG* arg = (ARG*)arg_;
    int argc = arg->sys_argc;
    char* argv1 = arg->sys_argv1;

    int local_socket_fd;
    struct sockaddr_in remote_address;

    local_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(local_socket_fd < 0){
        printf("create socket fail!\n");
        return -1;
    }

    memset(&remote_address, 0, sizeof(remote_address));
    remote_address.sin_family = AF_INET;
    remote_address.sin_addr.s_addr = inet_addr(argv1);
    remote_address.sin_port = htons(REMOTE_PORT); 

    udp_message_send(local_socket_fd, (struct sockaddr*)&remote_address);

    close(local_socket_fd);
    return 0;

}



void* receive_message_thread_function(void* arg_){

    ARG* arg = (ARG*)arg_;
    int argc = arg->sys_argc;
    char* argv1 = arg->sys_argv1;

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

    if(argc != 2){
        puts("usage: ./youchat <destination's IP address>");
        return 0;
    }

    ARG sys_arg;
    sys_arg.sys_argc = argc;
    sys_arg.sys_argv1 = &argv[1];

    puts("\n\n>========== YouChat version beta 1 is running exhaustively! ==========<\n\n");

    pthread_t send_message_thread;
    if(pthread_create(&send_message_thread, NULL, send_message_thread_function, (void*)&sys_arg) == 0)    puts("sender's thread is created successfully!\n");
    pthread_t receive_message_thread;
    if(pthread_create(&receive_message_thread, NULL, receive_message_thread_function, (void*)&sys_arg) == 0)    puts("receiver's thread is created successfully!\n");

    int send_message_thread_return, receive_message_thread_return;
    pthread_join(send_message_thread, &send_message_thread_return);
    pthread_join(receive_message_thread, &receive_message_thread_return);

    return 0;

}
