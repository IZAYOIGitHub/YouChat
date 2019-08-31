#include "common.h"

void account_to_destination_info(char* account, REGISTER_ADDR* register_users,struct sockaddr_in* client)
{
    int num = register_user->num;
    int i = 0;
    for (i = 0; i < num; i++ ){
        if(!strcmp(account, register_users->account_address[i].account)) {
            *client =  register_users->account_address[i].remote_addr;
            return ;
        }
    }
}