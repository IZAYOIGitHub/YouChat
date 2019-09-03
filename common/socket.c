#include "common.h"

void send_message_udp(struct sockaddr* destination_info_pointer,char* message_buffer)
{
    //这一部分加到调用代码的部分
	// int send_socket_fd;
	// if( (send_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    //         printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
    //         return 0;
    //     }

	// struct sockaddr_in destination_info;
	// printf("hello\n");
	// memset(&destination_info, 0, sizeof(destination_info));
	// destination_info.sin_family = AF_INET;
	// destination_info.sin_addr.s_addr = inet_addr(destination->IP_address);
	// destination_info.sin_port = htons(destination->port);
    int send_socket_fd;
    socklen_t len;
    send_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(send_socket_fd < 0)
    {
        printf("create socket fail!\n");
        return ;
    }
    len = sizeof(*destination_info_pointer);
    printf("client:%s\n",message_buffer);  //打印自己发送的信息
    sendto(send_socket_fd, message_buffer, 1024, 0, destination_info_pointer, len);
    close(send_socket_fd);
}
int send_message_tcp(struct sockaddr* destination_info,char* message_buffer){ // execute once, send message once

	int send_socket_fd;
    socklen_t len;
	if( (send_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
            printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
            return 0;
        }
	if( connect(send_socket_fd, (struct sockaddr*)&destination_info, sizeof(destination_info)) < 0){
		printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
		return 0;
	}

	if( send(send_socket_fd, message_buffer, strlen(message_buffer), 0) < 0){
		printf("send message error: %s(errno: %d)\n", strerror(errno), errno);
		return 0;
	}
	
	close(send_socket_fd);
	return 1;
}

int client_receive_message_udp(){  // once executed, always listening
	int listen_socket_fd;
	struct sockaddr_in local_info, remote_info; 
	char buffer[1024];
	socklen_t length = sizeof(remote_info);

	listen_socket_fd = socket(AF_INET, SOCK_DGRAM, 0); 
	if(listen_socket_fd < 0){
		printf("create socket fail!\n");
		return 0;
	}
	memset(&local_info, 0, sizeof(local_info));
	local_info.sin_family = AF_INET;
	local_info.sin_addr.s_addr = htonl(INADDR_ANY);
	local_info.sin_port = htons(8888); 

	if((bind(listen_socket_fd, (struct sockaddr*)&local_info, sizeof(local_info))) < 0){
		printf("socket bind fail!\n");
		return 0;
	}

	while(1){
		memset(buffer, 0, 1024);
		if((recvfrom(listen_socket_fd, buffer, 1024, 0, (struct sockaddr*)&remote_info, &length)) == -1){
			printf("recieve data fail!\n");
			return 0;
		}
		printf("remote:%s\n",buffer);
    	}

	close(listen_socket_fd);
	return 0;
}

int server_receive_udp(){  // once executed, always listening

	int listen_socket_fd;
	struct sockaddr_in local_info, remote_info; 
	char buffer[1024];
	socklen_t length = sizeof(remote_info);

	listen_socket_fd = socket(AF_INET, SOCK_DGRAM, 0); 
	if(listen_socket_fd < 0){
		printf("create socket fail!\n");
		return 0;
	}

	memset(&local_info, 0, sizeof(local_info));
	local_info.sin_family = AF_INET;
	local_info.sin_addr.s_addr = htonl(INADDR_ANY);
	local_info.sin_port = htons(8888); 

	if((bind(listen_socket_fd, (struct sockaddr*)&local_info, sizeof(local_info))) < 0){
		printf("socket bind fail!\n");
		return 0;
	}

	while(1){
		memset(buffer, 0, 1024);
		if((recvfrom(listen_socket_fd, buffer, 1024, 0, (struct sockaddr*)&remote_info, &length)) == -1){
			printf("recieve data fail!\n");
			return 0;
		}
		printf("client:%s\n",buffer);
		//--------------
		//下面函数功能未实现
		//---------------
		// ACCOUNT_MESSAGE accout_message;
		// split_message(buffer, &accout_message);
		// DESTINATION* client_2 = account_to_destination_info(accout_message.accout);
		// send_message_udp(client_2, accout_message.message);

    	}
	close(listen_socket_fd);
	return 0;
}

int server_receive_message_send_tcp(){ // once executed, always listening

	int  listen_socket_fd, connection_fd;
	struct sockaddr_in  local_info;
	char buffer[1024];
	int receive_message_length;

	if( (listen_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){   
		printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
		return 0;
	}

	memset(&local_info, 0, sizeof(local_info)); 
	local_info.sin_family = AF_INET;
	local_info.sin_addr.s_addr = htonl(INADDR_ANY);
	local_info.sin_port = htons(8888);   

	if( bind(listen_socket_fd, (struct sockaddr*)&local_info, sizeof(local_info)) == -1){   
		printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
		return 0;
	}

	if( listen(listen_socket_fd, 10) == -1){   
		printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
		return 0;
	}

	while(1){
		memset(buffer, 0, 1024);
        //阻塞，直到有新tcp客户端连接
		if( (connection_fd = accept(listen_socket_fd, NULL, NULL)) == -1){   
			printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
			continue;
		}
		recv(connection_fd, buffer, 1024, 0);
        printf("client:%s\n",buffer);

        //-----------------------------
        //未完成函数
        //----------------------------
		// ACCOUNT_MESSAGE accout_message;
		// split_message(buffer, &accout_message);
		// DESTINATION* client_2 = account_to_destination_info(accout_message.accout);
		// send_message_tcp(client_2, accout_message.message);

		close(connection_fd);
	}

	return listen_socket_fd;
}
int client_receive_message_tcp(){ // once executed, always listening

	int  listen_socket_fd, connection_fd;
	struct sockaddr_in  local_info;
	char buffer[1024];
	int receive_message_length;

	if( (listen_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){   
		printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
		return 0;
	}

	memset(&local_info, 0, sizeof(local_info)); 
	local_info.sin_family = AF_INET;
	local_info.sin_addr.s_addr = htonl(INADDR_ANY);
	local_info.sin_port = htons(6677);   

	if( bind(listen_socket_fd, (struct sockaddr*)&local_info, sizeof(local_info)) == -1){   
		printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
		return 0;
	}

	if( listen(listen_socket_fd, 10) == -1){   
		printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
		return 0;
	}

	while(1){
		memset(buffer, 0, 1024);
		if( (connection_fd = accept(listen_socket_fd, (struct sockaddr*)NULL, NULL)) == -1){   
			printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
			continue;
		}
		recv(connection_fd, buffer, 1024, 0);
		printf("receive message from remote: %s\n", buffer);
		close(connection_fd);
	}

	return listen_socket_fd;
}