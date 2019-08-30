#include "common.h"

CONNECTION* udp_set_connection(DESTINATION* destination){

	CONNECTION* udp_connection = malloc(sizeof(CONNECTION));

	udp_connection->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(udp_connection->socket_fd < 0){
		puts("create socket failed!");
		return NULL;
	}

	memset(&udp_connection->destination_info, 0, sizeof(udp_connection->destination_info));
	udp_connection->destination_info.sin_family = AF_INET;
	udp_connection->destination_info.sin_addr.s_addr = inet_addr(destination->IP_address);
	udp_connection->destination_info.sin_port = htons(destination->port);

	return udp_connection;
}


bool udp_send_message(CONNECTION* udp_connection, char* message_buffer){

	struct sockaddr* destination_info_pointer = (struct sockaddr*)&udp_connection->destination_info;
	socklen_t length = sizeof(*destination_info_pointer);
	sendto(udp_connection->socket_fd, message_buffer, BUFFER_SIZE, 0, destination_info_pointer, length);

}