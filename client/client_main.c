#include "common.h"


void* client_send_message_udp_thread(void* destination){
	char buffer[1024];

	puts("message format: <accout>$<message>");
	while(1){
		memset(buffer, 0, 1024);
		puts("local:");
		scanf("%s", buffer);
		client_send_message_udp((DESTINATION*)destination, buffer);
	}
}

int regist(DESTINATION* destination){
	char buffer[20];
	puts("Enter your account:");
	scanf("%s", buffer);
	client_send_message_udp(destination, buffer);
}

int main(int argc, char* argv[]){
	
	puts("\n\n>========== YouChat version beta 1 is running exhaustively! ==========<\n\n");

	DESTINATION server;
	puts("Please input server's IP address and connecting port:");
	scanf("%s", server.IP_address);
	scanf("%d", server.port);

	regist(&server);
	
	pthread_t send_message_thread;
    pthread_create(&send_message_thread, NULL, client_send_message_udp_thread, (void*)*server);
    pthread_t receive_message_thread;
    pthread_create(&receive_message_thread, NULL, client_receive_message_udp, NULL);

	int send_message_thread_return, receive_message_thread_return;
	pthread_join(send_message_thread, &send_message_thread_return);
	pthread_join(receive_message_thread, &receive_message_thread_return);

}
