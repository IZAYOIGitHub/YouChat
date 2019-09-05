#include <gtk/gtk.h>
#include "chat.h"
#include "chatwin.h"
#include "popup.h"
#include <stdlib.h>
#include <string.h>
#include "common.h"

int OpenChatWin(const char* ID);

static void SetBufferTag(ChatWin* chatwin);

static void SendMessage( GtkWidget *widget,  gpointer chatwin);

static void SendFile( GtkWidget *widget,  gpointer chatwin);

void PrintNewMsg(const gchar* ID, const gchar* msg, const gchar* time, int isgroup);

static void LockBuffer(ChatWin* chatwin);

static void UnlockBuffer(ChatWin* chatwin);

static void CloseChatWin( GtkWidget *widget, gpointer chatwin);

static void ChooseFont( GtkWidget *widget, gpointer chatwin);

static void ChooseColor( GtkWidget *widget, gpointer chatwin);

static void ChooseFile( GtkWidget *widget, gpointer chatwin);

int OpenChatWin(const gchar* ID)
{
	ChatWin* chatwin;
	int flag = CreatNewChatWin(&chatwin, ID);
	if (flag==0)
	{
		return 0;	
	}
	else if (flag==1)
	{
		gtk_label_set_label ((GtkLabel*)chatwin->label_ID, ID);	
		gtk_label_set_label ((GtkLabel*)chatwin->label_file, "选择文件：...");
		SetBufferTag(chatwin);

		//button_send
	        g_signal_connect(G_OBJECT(chatwin->button_send), "clicked",
        	        G_CALLBACK(SendMessage), (gpointer)chatwin);
		//button_close
		g_signal_connect(G_OBJECT(chatwin->button_close), "clicked",
                        G_CALLBACK(CloseChatWin), (gpointer)chatwin);
		//button_sendfile
		g_signal_connect(G_OBJECT(chatwin->button_sendfile), "clicked",
        	        G_CALLBACK(SendFile), (gpointer)chatwin);
		//colorbutton
		g_signal_connect(G_OBJECT(chatwin->colorbutton), "color-set",
        	        G_CALLBACK(ChooseColor), (gpointer)chatwin);
		//fontbutton
		g_signal_connect(G_OBJECT(chatwin->fontbutton), "font-set",
        	        G_CALLBACK(ChooseFont), (gpointer)chatwin);
		//filechooserbutton
		g_signal_connect(G_OBJECT(chatwin->filechooserbutton), "file-set",
        	        G_CALLBACK(ChooseFile), (gpointer)chatwin);
	}
	
	
	gtk_widget_show ((GtkWindow*)chatwin->window);

	return 1;
}

void CloseChatWin(GtkWidget *widget, gpointer chatwin)
{
	if (chatwin==NULL)
	{
		PopNotice(((ChatWin*)chatwin)->window, "错误", "关闭失败：窗口获取异常");
		return ;
	}

	DestoryChatWin(((ChatWin*)chatwin)->ID);
}

static void SetBufferTag(ChatWin* chatwin)
{
	GtkTextBuffer *buffer;
        buffer = chatwin->buffer_output;

	chatwin->right = gtk_text_buffer_create_tag(buffer, "right_justify", "justification", GTK_JUSTIFY_RIGHT, NULL);//居右
	//时间字体
	chatwin->timecolor = gtk_text_buffer_create_tag(buffer, NULL, "foreground", "blue", NULL);//蓝字
	chatwin->timefont = gtk_text_buffer_create_tag(buffer, NULL, "family", "Simhei", NULL);//黑体
	//默认用户字体
	chatwin->usercolor = gtk_text_buffer_create_tag(buffer, NULL, "foreground", "black", NULL);//黑字
	chatwin->userfont = gtk_text_buffer_create_tag(buffer, NULL, "family", "Sans", NULL);//默认字体 
}

static void ChooseFont( GtkWidget *widget, gpointer chatwin)
{
	/*
	GtkTextBuffer *buffer;
        buffer = ((ChatWin*)chatwin)->buffer_output;
	const gchar* font = gtk_font_chooser_get_font ((GtkFontButton*)(((ChatWin*)chatwin)->fontbutton));
	((ChatWin*)chatwin)->userfont = gtk_text_buffer_create_tag(buffer, NULL, "family", font, NULL);
	*/
	
	GtkTextBuffer *buffer;
        buffer = ((ChatWin*)chatwin)->buffer_output;

	const gchar* getfont = gtk_font_chooser_get_font ((GtkFontButton*)(((ChatWin*)chatwin)->fontbutton));
	gchar font[25];
	int i;	
	for ( i=0; getfont[i]!=' '; i++) font[i] = getfont[i];
	font[i] = '\0';

	((ChatWin*)chatwin)->userfont = gtk_text_buffer_create_tag(buffer, NULL, "family", font, NULL);
}

static void ChooseColor( GtkWidget *widget, gpointer chatwin)
{
	/*
	GtkTextBuffer *buffer;
        buffer = ((ChatWin*)chatwin)->buffer_output;
	const gchar* color = gtk_font_button_get_font_name ((GtkColorButton*)(((ChatWin*)chatwin)->colorbutton));
	((ChatWin*)chatwin)->usercolor = gtk_text_buffer_create_tag(buffer, NULL, "foreground", color, NULL);
	*/
	GtkTextBuffer *buffer;
        buffer = ((ChatWin*)chatwin)->buffer_output;
	((ChatWin*)chatwin)->usercolor = gtk_text_buffer_create_tag(buffer, NULL, "foreground", "red", NULL);
}


static void SendMessage( GtkWidget *widget,  gpointer chatwin)
{
	if (chatwin==NULL)
        {
                PopNotice(((ChatWin*)chatwin)->window, "错误", "发送失败：窗口获取异常");
		return ;
        }

	LockBuffer((ChatWin*)chatwin); //上锁

	GtkTextIter start, end;
	//提取消息
	GtkTextBuffer *buffer_input = ((ChatWin*)chatwin)->buffer_input;
	gtk_text_buffer_get_bounds (buffer_input, &start, &end);
	const gchar *buffer_msg = gtk_text_buffer_get_text (buffer_input, &start, &end, 1);
	if (strlen(buffer_msg)==0) return;
	gchar *msg = (gchar*)malloc(sizeof(buffer_msg));
	strcpy(msg, buffer_msg);
	
	gtk_text_buffer_set_text (buffer_input, "", -1);	

	GtkTextBuffer *buffer;
	buffer = ((ChatWin*)chatwin)->buffer_output;

	GtkTextMark *mark_timestart; //新信息时间开始位置，发送失败时插入红色感叹号提示
	mark_timestart = gtk_text_buffer_get_insert (buffer);
	
	//打印时间
	gchar time[50];
	time_now_to_string(time);
	gtk_text_buffer_get_end_iter(buffer,&end); 
	gtk_text_buffer_insert_with_tags(buffer, &end, time, -1,
		((ChatWin*)chatwin)->timecolor, ((ChatWin*)chatwin)->timefont, ((ChatWin*)chatwin)->right, NULL);
	gtk_text_buffer_insert_with_tags(buffer, &end, "\n", -1, ((ChatWin*)chatwin)->right, NULL);

	GtkTextMark *mark_timeend; //新信息时间结束位置，发送成功要更改为服务器时间
	mark_timeend = gtk_text_buffer_get_insert (buffer);
	
	//打印信息
	gtk_text_buffer_get_end_iter(buffer,&end); 
	gtk_text_buffer_insert_with_tags(buffer, &end, msg, -1,
		((ChatWin*)chatwin)->usercolor, ((ChatWin*)chatwin)->userfont, ((ChatWin*)chatwin)->right, NULL);
	gtk_text_buffer_insert_with_tags(buffer, &end, "\n", -1, ((ChatWin*)chatwin)->right, NULL);

	UnlockBuffer((ChatWin*)chatwin); //解锁

	tcp_message_send(((ChatWin*)chatwin)->ID, msg);

	free(msg);
}

static void LockBuffer(ChatWin* chatwin)
{
	if (chatwin==NULL)
        {
                PopNotice(((ChatWin*)chatwin)->window, "错误", "加锁失败：窗口获取异常");
                return ;
        }
	
	while(chatwin->buffer_lock==1); //等待解锁     
        chatwin->buffer_lock = 1;
}
static void UnlockBuffer(ChatWin* chatwin)
{
	if (chatwin==NULL)
        {
                PopNotice(((ChatWin*)chatwin)->window, "错误", "解锁失败：窗口获取异常");
                return ;
        }

	chatwin->buffer_lock = 0;
}

static int SendtoServer(const gchar *msg, ChatWin* chatwin, GtkTextMark *start, GtkTextMark *end)
{
	if (chatwin==NULL)
        {
                PopNotice(((ChatWin*)chatwin)->window, "错误", "发送服务端失败：窗口获取异常");
                return ;
        }

	return 0;
}

void PrintNewMsg(const gchar* ID, const gchar* msg, const gchar* time, int isgroup)
{
	/*if (chatwin==NULL)
        {
                PopNotice(((ChatWin*)chatwin)->window, "错误", "打印消息失败：窗口获取异常");
                return ;
        }*/

	ChatWin_Node* node = SearchManager(ID);
	if (node->next==NULL) return;
	ChatWin* chatwin = node->next->chatwin;

	LockBuffer(chatwin);
	
	GtkTextBuffer *buffer;
        buffer = chatwin->buffer_output;

	GtkTextIter end;
	
	
	gtk_text_buffer_get_end_iter(buffer,&end); 
	gtk_text_buffer_insert_with_tags(buffer, &end, ID, -1,
			chatwin->timecolor, chatwin->timefont, chatwin->right, NULL);
	gtk_text_buffer_insert_with_tags(buffer, &end, "@", -1,
			chatwin->timecolor, chatwin->timefont, chatwin->right, NULL);


	gtk_text_buffer_get_end_iter(buffer,&end); 
	gtk_text_buffer_insert_with_tags(buffer, &end, time, -1,
		chatwin->timecolor, chatwin->timefont, chatwin->right, NULL);
	gtk_text_buffer_insert_with_tags(buffer, &end, "\n", -1, chatwin->right, NULL);

	gtk_text_buffer_get_end_iter(buffer,&end); 
	gtk_text_buffer_insert_with_tags(buffer, &end, msg, -1,
		chatwin->usercolor, chatwin->userfont, chatwin->right, NULL);
	gtk_text_buffer_insert_with_tags(buffer, &end, "\n", -1, chatwin->right, NULL);

	UnlockBuffer(chatwin);
}

static void ChooseFile( GtkWidget *widget, gpointer chatwin)
{
	const gchar* file;
	file = gtk_file_chooser_get_filename ((GtkFileChooser*)(((ChatWin*)chatwin)->filechooserbutton));
	gtk_label_set_label ((GtkLabel*)(((ChatWin*)chatwin)->label_file), file);
}

static void SendFile( GtkWidget *widget,  gpointer chatwin)
{
	const gchar* file;
	file = gtk_label_get_label ((GtkLabel*)(((ChatWin*)chatwin)->label_file));
	//sendtoserver(file, (ChatWin*)chatwin->ID);
	gtk_label_set_label ((GtkLabel*)(((ChatWin*)chatwin)->label_file), "选择文件：...");
}
