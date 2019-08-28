#include<common_header.h>

int send_message(int socket_fd, char* string_buffer){
	
	if(send(socket_fd, string_buffer, strlen(string_buffer), 0) < 0){
		stderr("send message failed! Please check your internet connection or inform the developer debugging!");
		return 0;
	}
	return 1;
	
}
