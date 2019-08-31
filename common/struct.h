typedef struct account_message{
	char account[20];
	char message[1024];
}ACCOUNT_MESSAGE;


typedef struct {
	char account[20];
	struct sockaddr_in remote_addr;
}ACCOUNT_ADDR;

typedef struct register_address{
	int num;
	ACCOUNT_ADDR account_address[ACCOUNT_NUMBER];
}REGISTER_ADDR;