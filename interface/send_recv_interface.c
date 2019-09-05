#include "common.h"
#include "cJSON.h"
#include "gtk/gtk.h"

#define LOCAL_PORT 23334
#define SERVER_PORT 23333
#define BUFFER_SIZE 1024

extern struct sockaddr_in server;
extern int client_socket_fd;
extern char local_account[20];
char TIME[32];


void tcp_message_send(char *remote_account, char *message)
{
    socklen_t len;
    cJSON *json = cJSON_CreateObject();
    time_now_to_string(TIME);
    cJSON_AddNumberToObject(json,"type", 0);
    cJSON_AddStringToObject(json,"time",TIME);
    cJSON_AddStringToObject(json, "source", local_account);
    cJSON_AddStringToObject(json, "destination", remote_account);
    cJSON_AddStringToObject(json, "message", message);
    char *buffer = cJSON_Print(json);
    UpdateContactList((gchar *)remote_account, (gchar *)message);
    if (send(client_socket_fd, buffer, 1024, 0) < 0)
    {
        printf("send message error: %s(errno: %d)\n", strerror(errno), errno);
        return;
    }
}

void tcp_message_receive()
{
    char buf[1024];
    char source_account[20];
    char message[1024];
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        if (recv(client_socket_fd, buf, 1024, 0) < 0)
        {
            printf("fail to receive message.");
            return NULL;
        }
        // puts("here is me");
        cJSON *json = cJSON_Parse(buf);
        cJSON *json_source = cJSON_GetObjectItem(json, "source");
        cJSON *json_message = cJSON_GetObjectItem(json, "message");
        cJSON *json_time = cJSON_GetObjectItem(json, "time");
        // char *source_account = json_source->valuestring;
        // char *message = json_message->valuestring;
        memset(source_account,0,sizeof(source_account));
        memset(message,0,sizeof(message));
        // puts("here is me");
        printf("source:%s\n",json_source->valuestring);
        printf("message:%s\n",json_message->valuestring);
        strcpy(source_account,json_source->valuestring);
        strcpy(message,json_message->valuestring);
        // puts("copy_success");
        UpdateContactList((gchar *)source_account, (gchar *)message);
        //OpenChatWin(source_account);
        PrintNewMsg(source_account, message, json_time->valuestring,0);
    }
}

void *receive_message_thread_function()
{
    tcp_message_receive();
    return NULL;
}

void create_receive_thread()
{
    pthread_t receive_message_thread;
    pthread_create(&receive_message_thread, NULL, receive_message_thread_function, NULL);
    return;
}
