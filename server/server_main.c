#include "common.h"

#define handle_regist_request_PORT 23332;
#define handle_sign_in_request_PORT 23333;
#define transmit_single_chat_message_PORT 23334;

void handle_regist_request();
void handle_sign_in_request();
void transmit_single_chat_message();

int main(int argc, char* argv[]){
	puts("\n\n>========== YouChat server version beta 1 is running exhaustively! ==========<\n\n");

	pthread_t handle_regist_request_thread, handle_sign_in_request_thread, transmit_single_chat_message_thread;
	pthread_create(&handle_regist_request_thread, NULL,handle_regist_request , NULL);
	pthread_create(&handle_sign_in_request_thread, NULL,handle_sign_in_request , NULL);
	pthread_create(&transmit_single_chat_message_thread, NULL,transmit_single_chat_message , NULL);
	
	pthread_join(handle_regist_request_thread, NULL);
	pthread_join(handle_sign_in_request_thread, NULL);
	pthread_join(transmit_single_chat_message_thread, NULL);

	return 0;
}

void handle_regist_request(){
	
}