CFLAGS = -I ./common -I ./client -g `pkg-config --cflags --libs gtk+-3.0`

install:client/login.o client/mainPage.o client/StartChat.o common/cJSON.o common/common.o client/popup.o interface/send_recv_interface.o common/chat.o client/chatwin.o
	gcc $^ -o YouChat $(CFLAGS)
	rm common/*.o
	rm interface/*.o
	rm client/*.o

.c.o:
	gcc -c $< -o $@ $(CFLAGS)