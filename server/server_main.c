#include "common.h"

void server_receive_message_send_udp_thread(void* regist_addr)
{
	REGISTER_ADDR* all_addr = (REGISTER_ADDR*)regist_addr;
	server_receive_message_send_udp(all_addr);
}

void server_receive_message_send_tcp_thread(void* regist_addr)
{
	REGISTER_ADDR* all_addr = (REGISTER_ADDR*)regist_addr;
	server_receive_message_send_tcp(all_addr);
}

int main(int argc, char* argv[]){
	puts("\n\n>========== YouChat server version beta 1 is running exhaustively! ==========<\n\n");

	REGISTER_ADDR register_user;
	register_user.num = 0;
	pthread_t server_udp_thread;
	pthread_create(&server_udp_thread, NULL,server_receive_message_send_udp_thread, &register_user);
	pthread_t server_tcp_thread;
	pthread_create(&server_tcp_thread, NULL,server_receive_message_send_tcp_thread, &register_user);
	
	int udp_return, tcp_return;
	pthread_join(server_udp_thread, &udp_return);
	pthread_join(server_tcp_thread, &tcp_return);
}

