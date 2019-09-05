#include<gtk/gtk.h>

#ifndef CHATWIN_H
#define CHATWIN_H

typedef struct chatwin //聊天窗体
{
	char* ID; //聊天对象ID
	GtkWidget *label_ID;
	GtkWidget *window;
	GtkWidget *view_output;
	GtkTextBuffer *buffer_output;
	GtkWidget *view_input;
	GtkTextBuffer *buffer_input;
	GtkWidget *button_send;
	GtkWidget *button_close;
	GtkWidget *button_sendfile;
	GtkWidget *label_file;
	GtkWidget *filechooserbutton;
	GtkWidget *colorbutton;
	GtkWidget *fontbutton;
	GtkTextTag *userfont;
	GtkTextTag *usercolor;
	GtkTextTag *timefont;
	GtkTextTag *timecolor;
	GtkTextTag *right;
	
	int buffer_lock; //输出界面锁，1表上锁，0表未锁
}ChatWin;

/*
ID=-1指示头结点
*/
typedef struct chatwin_manager
{
	char* ID;
	ChatWin *chatwin;
	struct chatwin_manager *next;
}ChatWin_Manager, ChatWin_Node;

int CreatNewChatWin(ChatWin* *p_chatwin, const char* ID);

void DestoryChatWin(const char* ID);

static void InitManager();

ChatWin_Node* SearchManager(const char* ID); //查找对应窗口，存在返回其前驱，不存在返回最后一个结点

static void AddtoManager(ChatWin* chatwin, const char* ID);

#endif
