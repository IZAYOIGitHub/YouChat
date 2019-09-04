#include "common.h"
#include "cJSON.h"
#include "gtk/gtk.h"

#define LOCAL_PORT 23334
#define SERVER_PORT 23333
#define BUFFER_SIZE 1024

extern struct sockaddr_in server;
extern int client_socket_fd;
extern char  local_account[20];

int check_account_exist(char* remote_account)
{
        socklen_t len;
        int type = 3;
        cJSON* json =  cJSON_CreateObject();
//        cJSON_AddNumberToObject(json,"type", type);
        cJSON_AddStringToObject(json, "destination", remote_account);
        char* buffer = cJSON_Print(json);
        if( send(client_socket_fd, buffer, strlen(buffer) + 1, 0) < 0){
		    printf("send message error: %s(errno: %d)\n", strerror(errno), errno);
            return -1;
        }
        char buf[10];
        if(recv(client_socket_fd,  buf, 10, 0) <  0) {
            printf("receive failed\n");
            return -1;
        }
        int tag  = 0;
        if(strcmp(buf, "exist") == 0) tag = 1;
        return tag;
}

void tcp_message_send(char* remote_account, char* message)
{
    socklen_t len;

        memset(message, 0, sizeof(message));
        scanf("%s",message);
        cJSON* json =  cJSON_CreateObject();
 //       cJSON_AddNumberToObject(json,"type", type);
        cJSON_AddStringToObject(json,"source", local_account);
        cJSON_AddStringToObject(json, "destination", remote_account);
        cJSON_AddStringToObject(json, "message", message);
        char* buffer = cJSON_Print(json);
         UpdateContactList((gchar*)remote_account, (gchar*)message);
        if( send(client_socket_fd, buffer, strlen(buffer) + 1, 0) < 0){
		    printf("send message error: %s(errno: %d)\n", strerror(errno), errno);
            return ;
        }	
}

void tcp_message_receive()
{
    char buf[1024];
     while(1)
    {
        memset(buf,0,sizeof(buf));
        if(recv(client_socket_fd, buf , 1024,  0) < 0){
            printf("fail to receive message.");
            return NULL;
        }
        cJSON* json = cJSON_Parse(buf);
        cJSON* json_source = cJSON_GetObjectItem(json, "source");
        cJSON* json_message = cJSON_GetObjectItem(json, "message");
        char* source_account = json_source->valuestring;
        char* message = json_message->valuestring;
        UpdateContactList((gchar*)source_account, (gchar*)message);
        PrintNewMsg( (gchar*)source_account,(gchar*)message,(gchar* time)));
    }
}

void* receive_message_thread_function(){
    tcp_message_receive();
    return NULL;
}

void create_receive_thread()
{
    pthread_t receive_message_thread;
    pthread_create(&receive_message_thread, NULL, receive_message_thread_function, NULL);
    int  receive_message_thread_return;
    pthread_join(receive_message_thread, &receive_message_thread_return);
    return ;
}

