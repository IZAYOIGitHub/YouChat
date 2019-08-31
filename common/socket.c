#include "common.h"

int send_message_udp(struct sockaddr_in destination_info, char* message_buffer){ // execute once, send message once

	int send_socket_fd;
	if( (send_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
            printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
            return 0;
        }

	struct sockaddr* destination_info_pointer = (struct sockaddr*)&destination_info;
	socklen_t length = sizeof(*destination_info_pointer);
	if( sendto(send_socket_fd, message_buffer, 1024, 0, destination_info_pointer, length) < 0){
		printf("send message error: %s(errno: %d)\n", strerror(errno), errno);
        	return 0;
	}

	close(send_socket_fd);
	return 1;
}

int send_message_tcp(struct sockaddr_in destination_info, char* message_buffer){ // execute once, send message once

	int send_socket_fd;
	if( (send_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
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
