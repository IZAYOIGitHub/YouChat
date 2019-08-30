#include "common.h"

bool send_message_udp(int socket_fd, DESTINATION* destination, char* message_buffer){

	struct sockaddr_in destination_info;
	memset(&destination_info, 0, sizeof(destination_info));
	destination_info.sin_family = AF_INET;
	destination_info.sin_addr.s_addr = inet_addr(destination->IP_address);
	destination_info.sin_port = htons(destination->port);

	struct sockaddr* destination_info_pointer = (struct sockaddr*)&destination_info;
	socklen_t length = sizeof(*destination_info_pointer);
	sendto(socket_fd, message_buffer, BUFFER_SIZE, 0, destination_info_pointer, length);

	return true;
}
