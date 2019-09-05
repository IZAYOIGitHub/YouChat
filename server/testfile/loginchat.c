#include "common.h"
#include "cJSON.h"

#define LOCAL_PORT 23334
#define SERVER_PORT 23333
#define BUFFER_SIZE 1024

struct sockaddr_in server;
int client_socket_fd;

typedef struct sock_remote_account{
    char remote_account[20];  //需要发送的用户名
    char source_account[20];  //来源
    int fd;  //需要发送人的套接字
}SOCK_ACCOUNT;

char TIME[1024];

int send_message_tcp(char* buffer){ // execute once, send message once

	if( send(client_socket_fd, buffer, strlen(buffer), 0) < 0){
		printf("send message error: %s(errno: %d)\n", strerror(errno), errno);
		return 0;
	}	
	return 1;
}

//下线函数
int logout(SOCK_ACCOUNT* SRA)
{
    cJSON* json=cJSON_CreateObject();
    time_now_to_string(TIME);
    printf("current time:%s\n",TIME);
    cJSON_AddNumberToObject(json,"type",2);
    cJSON_AddItemToObject(json,"source",cJSON_CreateString(SRA->source_account));
    cJSON_AddItemToObject(json,"destination",cJSON_CreateString("password"));
    cJSON_AddItemToObject(json,"message",cJSON_CreateString("NULL"));
    cJSON_AddStringToObject(json, "time", TIME);
    char* buf = cJSON_Print(json);
    // printf("datalogout:%s\n",buf);
    send_message_tcp(buf);
    printf("send ok\n");
    //接收服务端返回信息
    recv(client_socket_fd, buf  ,1024,0);
    printf("recv data of my world is: %s\n",buf);
    if(strcmp(buf,"logout_success") == 0)
    {
        // close(client_socket_fd);
        return 0;
    }
    else
    {
        return 1;
    }
    cJSON_Delete(json);
}


void tcp_message_send(SOCK_ACCOUNT* SRA)
{
    socklen_t len;
    char message[BUFFER_SIZE];
    int type;
    char remote_account[20];
    printf("输入目标账名/群名：");
    scanf("%s",remote_account);
    strcpy(SRA->remote_account,remote_account);
    printf("单聊/群聊：");
    scanf("%d",&type);
    printf("输入消息：");
    while(1)
    {
        memset(message, 0, sizeof(message));
        scanf("%s",message);
        cJSON* json =  cJSON_CreateObject();
        time_now_to_string(TIME);
        printf("current time:%s\n",TIME);
        cJSON_AddNumberToObject(json,"type", type);
        cJSON_AddStringToObject(json,"source", SRA->source_account);
        cJSON_AddStringToObject(json, "destination", SRA->remote_account);
        cJSON_AddStringToObject(json, "message", message);
        cJSON_AddStringToObject(json, "time", TIME);
        char* buffer = cJSON_Print(json);
        if( send(client_socket_fd, buffer, strlen(buffer) + 1, 0) < 0){
		    printf("send message error: %s(errno: %d)\n", strerror(errno), errno);
            logout((SOCK_ACCOUNT*)SRA);
            return ;
        }
    }	
}

void* send_message_thread_function(void* SRA){
    tcp_message_send((SOCK_ACCOUNT*)SRA);
    return NULL;
}

void tcp_message_receive(SOCK_ACCOUNT* SRA)
{
    char buf[1024];
     while(1)
    {
        memset(buf,0,sizeof(buf));
        // printf("ready to receive buf： %s , %d\n",buf,client_socket_fd);
        if(recv(client_socket_fd, buf,1024,  0) < 0){
            printf("fail to receive message.");
            logout((SOCK_ACCOUNT*)SRA);
            return;
        }
        else
        {
            printf("datarecv:%s\n",buf);
            printf("hello\n");
        }
    }
}

void* receive_message_thread_function(void* SRA){
    tcp_message_receive((SOCK_ACCOUNT*)SRA);
    return NULL;
}

void create_chat_thread(char* source_account)
{
    
    SOCK_ACCOUNT SRA;  //账户名，套接字结构体
    strcpy(SRA.source_account,source_account);

    pthread_t send_message_thread;
    pthread_create(&send_message_thread, NULL, send_message_thread_function, (void*)&SRA);
    pthread_t receive_message_thread;
    pthread_create(&receive_message_thread, NULL, receive_message_thread_function, (void*)&SRA);
    int send_message_thread_return, receive_message_thread_return;
    pthread_join(send_message_thread, &send_message_thread_return);
    pthread_join(receive_message_thread, &receive_message_thread_return);
    return ;
}



//发送信息



int client_login(char* account,char* password )
{

    cJSON* json=cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"type",1);
    cJSON_AddItemToObject(json,"username",cJSON_CreateString(account));
    cJSON_AddItemToObject(json,"password",cJSON_CreateString(password));
    
    char* buf = cJSON_Print(json);
    // printf("datalogin:%s\n",buf);
    memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	// server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_addr.s_addr = inet_addr("192.168.43.32");
	server.sin_port = htons(23333);
    socklen_t len;
//     printf("hello\n");
	if( (client_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
            printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
             cJSON_Delete(json);
            return 0;
        }
	if( connect(client_socket_fd, (struct sockaddr*)&server, sizeof(server)) < 0){
		printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        cJSON_Delete(json);
		return 0;
	}
    //发送账户
//     printf("hello\n");
    send_message_tcp(buf);
    printf("send ok\n");
    //接收服务端返回信息
    recv(client_socket_fd,buf,1024,0);
    printf("recv data of my world is: %s\n",buf);
    if(strcmp(buf,"error") == 0 || strcmp(buf,"login_fail")==0 || strcmp(buf,"you_have_already_online") == 0)
    {
        close(client_socket_fd);
        cJSON_Delete(json);
        return 0;
    }
    else
    {
        return 1;
        cJSON_Delete(json);
    }
}

int main()
{
    char user_id[20];
    char psd[20];
    SOCK_ACCOUNT temp;
    scanf("%s",&user_id);
    scanf("%s",&psd);
    client_login(user_id,psd);
    create_chat_thread(user_id);
    strcpy(temp.source_account,user_id);
    printf("输入指令");
    char cmd[40];
    scanf("%s",cmd);
    if(strcmp(cmd,"logout")==0)
        logout(&temp);

//     修改
//     source_account=user_id;
    return 0;
}