int split_message(char* origin_message,ACCOUNT_MESSAGE* account_message)
{
    int origin_size = strlen(origin_message);
    int account_size = 0, message_size = 0, i = 0;
    for(i = 0; i < origin_size; i++) {
        if(origin_message[i] == '$') {
            account_size = i;
            message_size = origin_size - account_size - 1;
            break;
        }
    }
    strncpy(account_message->account, origin_message, account_size);
	account_message->account[account_size] = '\0';
    strncpy(account_message->message, origin_message + account_size + 1, message_size);
	account_message->message[message_size] = '\0';
    return 0;
}
