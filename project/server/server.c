#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "cJSON.h"

typedef struct account{
    int  connection_fd;
	char username[20];
	char password[20];
}ACCOUNT;

int account_num = 0;
ACCOUNT account_table[100];

void* service(void* account_p){
    ACCOUNT* account_pointer = (ACCOUNT*)account_p;
    int no_match = 1;

    while(1){
        char json_buffer[1024];
        memset(json_buffer, 0, 1024);
        recv(account_pointer->connection_fd, json_buffer, 1024, 0);
        puts(json_buffer);
        cJSON* json_data = cJSON_Parse(json_buffer);
        cJSON* json_type = cJSON_GetObjectItem(json_data, "type");
        cJSON* json_destination = cJSON_GetObjectItem(json_data, "destination");
        switch(json_type->valueint){
            case 0: // single chat
                for(int i=1; i<=account_num; i++){
                    if(strcmp(account_table[i].username, json_destination->valuestring) == 0){
                        send(account_table[i].connection_fd, json_buffer, 1024, 0);
                        puts("transmit OK!\n\n");
                        break;
                    }
                }
                break;
            case 1: // multiple chat
                for(int i=1; i<=account_num; i++){
                    if(account_table[i].connection_fd != 0)
                        send(account_table[i].connection_fd, json_buffer, 1024, 0);
                }
                break;
            case 2: //logout
                puts("logout_success!\n\n");
                send(account_pointer->connection_fd, "logout_success", 15, 0);
                close(account_pointer->connection_fd);
                account_pointer->connection_fd = 0;
                pthread_exit(0);
                return NULL;
            case 3:
                for(int i=1; i<account_num; i++){
                    if(strcmp(account_table[i].username, json_destination->valuestring) == 0){
                        send(account_pointer->connection_fd, "exist", 6, 0);
                        no_match = 0;
                        break;
                    }
                }
                if(no_match)    send(account_pointer->connection_fd, "not_exist", 10, 0);
                break;
        }
    }

}


int main(){

    int no_match;
    pthread_t servive_thread_fd;

    puts("============YouChat super server is running exhaustively!============\n\n");
    bzero(account_table, sizeof(account_table));

    // bind
    int listen_socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
    struct sockaddr_in local_info;
    memset(&local_info, 0, sizeof(local_info));
    local_info.sin_family = AF_INET;
    local_info.sin_addr.s_addr = htonl(INADDR_ANY);
    local_info.sin_port = htons(23333); 
    bind(listen_socket_fd, (struct sockaddr*)&local_info, sizeof(local_info));

    // listen
    listen(listen_socket_fd, 10);

    // while(1)
    while(1){   
        // accept
        struct sockaddr_in client_info;  
        int length = sizeof(client_info);  
        int connection_fd = accept(listen_socket_fd, (struct sockaddr *)&client_info, &length);

        // register or log in ?
        char json_buffer[1024];
        memset(json_buffer, 0, 1024);
        recv(connection_fd, json_buffer, 1024, 0);
        puts(json_buffer);
        cJSON* json_data = cJSON_Parse(json_buffer);
        cJSON* json_type = cJSON_GetObjectItem(json_data, "type");
        cJSON* json_username = cJSON_GetObjectItem(json_data, "username");
        cJSON* json_password = cJSON_GetObjectItem(json_data, "password");

        switch(json_type->valueint){
            case 0: // register
                account_num++;
                strcpy(account_table[account_num].username, json_username->valuestring);
                strcpy(account_table[account_num].password, json_password->valuestring);
                puts("register_success!\n\n");
                send(connection_fd, "register_success", 17, 0);
                close(connection_fd);
                break;
            case 1: // login
                no_match = 1;
                for(int i=1; i<=account_num; i++){
                    if(   (strcmp(account_table[i].username, json_username->valuestring) == 0)  &&  (strcmp(account_table[i].password, json_password->valuestring) == 0)   ){
                        no_match = 0;
                        account_table[i].connection_fd = connection_fd;
                        puts("login_success!\n\n");
                        send(connection_fd, "login_success", 14, 0);
                        // service()
                        pthread_create(&servive_thread_fd, NULL, service, (void*)&account_table[i]);
                        break;
                    } 
                }
                if(no_match){
                    send(connection_fd, "login_fail", 11, 0);
                    close(connection_fd);
                }
                break;
        } 
    }
}













