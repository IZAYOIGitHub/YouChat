#include "common.h"

void split_message(char* origin_message,AccountAndMessage* A_M)
{
    int origin_size = strlen(origin_message);
    int Account_size = 0, Message_size = 0, i = 0;
    for(i = 0; i < origin_size; i++) {
        if(origin_message[i] == '$') {
            Account_size = i;
            Message_size = origin_size - Account_size;
            break;
        }
    }
    A_M->Account = (char*)malloc((Account_size + 1) * sizeof(char));
    A_M->Message = (char*)malloc((Message_size + 1) * sizeof(char));
    int j = 0;
    for (j = 0; j < Account_size; j++) {
        A_M->Account[j] = origin_message[j];
    }
    A_M->Account[j] = 0;
    for(j = 0;j + i <= origin_size; j++) {
        A_M->Message[j] = origin_message[j + i];
    }
    return ;
}