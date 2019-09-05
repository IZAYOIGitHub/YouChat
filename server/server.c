#include "../common/common.h"
#include "../common/cJSON.h"

typedef struct account{
    int  connection_fd;
	char username[20];
	char password[20];
}ACCOUNT;

typedef struct Outmessage{
    char source_id[20];
    char destination_id[20];
    char message[200];
    char time_send[32]; //发送时间
}OUTMESSAGE;

int account_num = 0;
ACCOUNT account_table[100];
MYSQL mysql;



void chat(int client_sockfd,char* json_buffer)
{
    // single chat
    
    //开始在在线好友中查找
    cJSON* json_data = cJSON_Parse(json_buffer);
    cJSON* json_destination = cJSON_GetObjectItem(json_data, "destination");
    cJSON* json_message = cJSON_GetObjectItem(json_data,"message");
    cJSON* json_time = cJSON_GetObjectItem(json_data,"time");
    cJSON* json_source_id = cJSON_GetObjectItem(json_data,"source");
    if(strcmp(json_destination->valuestring,"group"))
    {
        printf("here is single chat\n");
        //在线的话就发出去
        for(int i=1; i<=account_num; i++){
            if(strcmp(account_table[i].username, json_destination->valuestring) == 0){
                send(account_table[i].connection_fd, json_buffer, 1024, 0);
                puts("transmit OK!\n\n");
            }
        }
        //不管在不在线，都记录消息,不在线就是离线消息
        //记录source_id,destination_id,message
        MYSQL_RES *res;
        MYSQL_ROW row;
        //离线信息的存储形式
        OUTMESSAGE Message;
        char sqlcmd[1024];
            
        //这里需要判断一下该好友是否注册
        sprintf(sqlcmd,"select * from user_info where user_id='%s'",json_destination->valuestring);

        //find id and password
        int temp = mysql_real_query(&mysql,sqlcmd,(unsigned int)strlen(sqlcmd));
        if(temp)
        {
            fprintf(stderr,"query error:%s\n",mysql_error(&mysql));
            //响应无此人
            // send(client_sockfd, "The_ID_don't_register", 24, 0);
            return;
        }
        res=mysql_store_result(&mysql);
        mysql_free_result(res);
        //否则就可以开始记录信息
        strcpy(Message.destination_id,json_destination->valuestring);
        strcpy(Message.message,json_message->valuestring);
        strcpy(Message.time_send,json_time->valuestring);
        strcpy(Message.source_id,json_source_id->valuestring);
        //存入数据库
        //最后一个0是标记状态，0代表还未发送给指定用户
        sprintf(sqlcmd,"insert into outmessage value('%s','%s','%s','%s')",Message.source_id,Message.destination_id,Message.message,Message.time_send);
        if(mysql_query(&mysql,sqlcmd))
        {
            fprintf(stderr,"query error:%s\n",mysql_error(&mysql));
        }
        else 
        {
            puts("save_message_success!\n");
        }
        return;
    }
    //group_chat
    else{
        printf("here is group chat\n");
        
        for(int i=1; i<=account_num; i++){
                    if(account_table[i].connection_fd != 0 && strcmp(account_table[i].username,json_source_id->valuestring))
                    {
                        
                        send(account_table[i].connection_fd, json_buffer, 1024, 0);
                        printf("destination:%s\n",account_table[i].username);
                        puts(json_buffer);
                    }
                }
        return;
    }
}

void* service(void* account_p){
    ACCOUNT* account_pointer = (ACCOUNT*)account_p;

    while(1){
        char json_buffer[1024];
        memset(json_buffer, 0, 1024);
        recv(account_pointer->connection_fd, json_buffer, 1024, 0);
        puts(json_buffer);
        cJSON* json_data = cJSON_Parse(json_buffer);
        cJSON* json_type = cJSON_GetObjectItem(json_data, "type");
        cJSON* json_username = cJSON_GetObjectItem(json_data,"source");

        // cJSON* json_destination = cJSON_GetObjectItem(json_data, "destination");
        switch(json_type->valueint){
            case 0: // single chat
                chat(account_pointer->connection_fd,json_buffer);
                break;
            case 2: //logout
                printf("currcent logout user:%s\n",json_username->valuestring);

                for(int i = 1; i <= account_num ; i++)
                {
                    if(strcmp(json_username->valuestring,account_table[i].username)==0)
                    {
                        printf("%s\n",account_table[i].username);
                        for(int j = i ; j <= account_num ; j++)
                        {
                            //删除该在线用户
                            strcpy(account_table[j].username,account_table[j+1].username);
                            strcpy(account_table[j].password,account_table[j+1].password);
                            account_table[j].connection_fd = account_table[j+1].connection_fd;
                        }
                        // send(account_pointer->connection_fd, "logout_success", 15, 0);
                        // printf("send ok\n");
                        account_pointer->connection_fd = 0;
                        printf("logout_success\n");
                        pthread_exit(0);
                        close(account_pointer->connection_fd);

                        //currcent user
                        for(int flag = 1 ; flag <= account_num ; flag++)
                        {
                            printf("user %d:%s\n",flag,account_table[flag].username);
                        }
                        return NULL;
                    }
                }
                account_num--;
                // send(account_pointer->connection_fd,"logout_fail",15,0);
                break;
        }
    }

}
//初始化sql
void sql_manage()
{
	static char *server_args[]={"this_program",
	"--datadir=.",
	"--key_buffer_size=32M"};
	static char *server_groups[]={"embeded","server","this_program_SERVER",(char *)NULL};

	if(mysql_library_init(sizeof(server_args)/sizeof(char *),server_args,server_groups))
	exit(1);
	mysql_init(&mysql);
	mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"your_prog_name");
	if(!mysql_real_connect(&mysql,"localhost","root","DB258011","TEST",0,NULL,0))
	fprintf(stderr,"can not connect MYSQL:%s\n",mysql_error(&mysql));
	else puts("MySQL connect successfully");
}
void register_server(int client_sockfd,cJSON* json_data)
{
    char sqlcmd[1024];
    ACCOUNT User;
    //将信息提出
    cJSON* json_username = cJSON_GetObjectItem(json_data, "username");
    cJSON* json_password = cJSON_GetObjectItem(json_data, "password");

    //add to database
    strcpy(User.username,json_username->valuestring);
    strcpy(User.password,json_password->valuestring);
    printf("register_ID:%s\n",User.username);
    printf("register_password:%s\n",User.password);
    sprintf(sqlcmd,"insert into user_info values('%s','%s')",User.username,User.password);
    //打印注册成功信息
    // printf("json_username:%s\n",json_username->valuestring);
    // printf("json_password:%s\n",json_password->valuestring);
    if(mysql_query(&mysql,sqlcmd))
	{
		fprintf(stderr,"query error:%s\n",mysql_error(&mysql));
		send(client_sockfd ,"register_error",15,0);
	}
	else 
    {
        puts("register_success!\n\n");
        //给予客户端回响
        send(client_sockfd, "register_success", 17, 0);
    }
    //同时账户的数量加一
    close(client_sockfd);
}
void send_outline(int client_sockfd,cJSON* temp_account)
{
    MYSQL_RES *res;
	MYSQL_ROW row;
    char sqlcmd[1024];
    char buf[1024];
    sprintf(sqlcmd,"select source_id,message,time_send from outmessage where destination_id='%s'",temp_account->valuestring);
    int temp = mysql_real_query(&mysql,sqlcmd,(unsigned int)strlen(sqlcmd));
    if(temp)
    {
        fprintf(stderr,"query error:%s\n",mysql_error(&mysql));
        return;
    }
    else
    {
        res=mysql_store_result(&mysql);
        printf("res:%d\n",res->field_count);
        int i=0;
        while((row = mysql_fetch_row(res)) )
        {       
            // send(client_sockfd,"history_message",16,0);   
            cJSON* json =  cJSON_CreateObject();
            printf("source:%s\n",row[0]);
            printf("message:%s\n",row[1]);
            printf("time:%s\n",row[2]);
            cJSON_AddStringToObject(json,"source", row[0]);
            cJSON_AddStringToObject(json, "message", row[1]);
            cJSON_AddStringToObject(json, "time", row[2]);
            char* buffer = cJSON_Print(json);
            printf("time:%d\nbuffer:%s\n",i,buffer);
            //可能跟字数有关
            send(client_sockfd,buffer,1024,0);
            i++;
        }
        mysql_free_result(res);     
    }

}
void login_server(int client_sockfd,cJSON* json_data)
{
    //登录后开启线程
    pthread_t servive_thread_fd;
    MYSQL_RES *res;
	MYSQL_ROW row;
    char sqlcmd[1024];
    
    // printf("currcent:%d\n",client_sockfd);
    cJSON* json_username = cJSON_GetObjectItem(json_data, "username");
    cJSON* json_password = cJSON_GetObjectItem(json_data, "password");

    //判断下是否已经在线
    for(int i = 1 ; i <= account_num ;i++)
    {
        if(strcmp(account_table[i].username,json_username->valuestring) == 0)
        {
            // send(client_sockfd, "you_have_already_online", 25, 0);
            puts("you_have_already_online");
            // close(client_sockfd);
            return;
        }
    }
    // printf("hello\n");
    //write mysql cmd
    sprintf(sqlcmd,"select * from user_info where user_id='%s' and user_password='%s'",json_username->valuestring,json_password ->valuestring);
    printf("login_ID:%s\n",json_username->valuestring);
    printf("login_password:%s\n",json_password->valuestring);

    //find id and password
    int temp = mysql_real_query(&mysql,sqlcmd,(unsigned int)strlen(sqlcmd));
    if(temp)
    {
        fprintf(stderr,"query error:%s\n",mysql_error(&mysql));
        return;
    }
    else
    {
        res=mysql_store_result(&mysql);
		row=mysql_fetch_row(res);
        if(row)
		{
			printf("login_success\n");
            send(client_sockfd, "login_success", 15, 0);
            account_num++;
            printf("currcent user:%d\n",account_num);
            account_table[account_num].connection_fd =  client_sockfd;
            strcpy(account_table[account_num].username,json_username->valuestring);
            strcpy(account_table[account_num].password,json_password->valuestring);
            //线程
            pthread_create(&servive_thread_fd, NULL, service, (void*)&account_table[account_num]);
             //currcent user
            for(int flag = 1 ; flag <= account_num ; flag++)
            {
                printf("user %d:%s\n",flag,account_table[flag].username);
            }
            // //add the fd to user_info
            // sprintf(sqlcmd,"UPDATE user_info SET user_fd=%d where user_id= '%s'",client_sockfd,json_username->valuestring);
            // mysql_query(&mysql,sqlcmd);
            
            // send_outline(client_sockfd,json_username);       
		}
		else
		{
			printf("login error\n");
			send(client_sockfd ,"login_fail\n", 11,0);
            close(client_sockfd);
            return;
		}
		mysql_free_result(res);
    }
    return;
}
int main(){

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
    //init mysql
    sql_manage(); 
    // listen
    listen(listen_socket_fd, 10);

    // while(1)
    while(1){   
        // accept
        struct sockaddr_in client_info;  
        int length = sizeof(client_info);  
        int connection_fd = accept(listen_socket_fd, (struct sockaddr *)&client_info, &length);
        char json_buffer[1024];
        memset(json_buffer, 0, 1024);
        //接收注册信息
        recv(connection_fd, json_buffer, 1024, 0);
        //调试打印注册信息
        // puts("json_buffer:\n");
        puts(json_buffer);
        //cJSON格式解析
        cJSON* json_data = cJSON_Parse(json_buffer);
        cJSON* json_type = cJSON_GetObjectItem(json_data, "type");
        cJSON* json_username = cJSON_GetObjectItem(json_data,"username");
        // register or log in ?

        switch(json_type->valueint){
            case 0: // register
                register_server(connection_fd,json_data);
                break;
            case 1: // login
                login_server(connection_fd,json_data);
                break;
        } 
    }
}













