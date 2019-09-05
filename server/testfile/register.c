#include "common.h"
#include "cJSON.h"


struct sockaddr_in server;
//发送信息
int send_message_tcp(int send_socket_fd,char* buffer){ // execute once, send message once

	if( send(send_socket_fd, buffer, strlen(buffer), 0) < 0){
		printf("send message error: %s(errno: %d)\n", strerror(errno), errno);
		return 0;
	}	
	return 1;
}
//注册
int client_register(char* account,char* password)
{
    
    cJSON* json=cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"type",0);
    cJSON_AddItemToObject(json,"username",cJSON_CreateString(account));
    cJSON_AddItemToObject(json,"password",cJSON_CreateString(password));

    char* buf = cJSON_Print(json);
    // printf("data:%s\n",buf);
    memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
    // server.sin_addr.s_addr = inet_addr("192.168.43.97");
	server.sin_port = htons(23333);
    int client_socket_fd;
    socklen_t len;
	if( (client_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
            printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
            return 0;
        }
	if( connect(client_socket_fd, (struct sockaddr*)&server, sizeof(server)) < 0){
		printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
		return 0;
	}
    
    //发送账户
    send_message_tcp(client_socket_fd,buf);
    // printf("send ok\n");
    //接收服务端返回信息
    recv(client_socket_fd,buf,1024,0);
    // printf("recv data of my world is: %d\n",length);
    cJSON_Delete(json);
    if(strcmp(buf,"error") == 0)
    {
        close(client_socket_fd);
        return 0;
    }
    else if(strcmp(buf,"success") == 0)
    {
        close(client_socket_fd);
        return 1;
    }
}

int main()
{
    char user_id[20];
    char psd[20];
    scanf("%s",&user_id);
    scanf("%s",&psd);
    client_register(user_id,psd);
//     修改
//     source_account=user_id;
    return 0;
}