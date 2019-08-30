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
	local_info.sin_port = htons(8899); 

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