#server
gcc -o server server.c ../common/cJSON.c ../common/common.c -L /usr/lib64/mysql -lmysqlclient -lpthread 

