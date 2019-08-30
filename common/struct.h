#include "common.h"

typedef struct destination{
	char* IP_address;
	int port;
}DESTINATION;

typedef struct connection{
	int socket_fd;
	struct sockaddr_in destination_info;
}CONNECTION;
