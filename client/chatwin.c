#include <gtk/gtk.h>
#include "chat.h"
#include "chatwin.h"
#include <stdlib.h>

static ChatWin_Manager *manager = NULL;

int CreatNewChatWin(ChatWin* *p_chatwin, const char* ID);

void DestoryChatWin(const char* ID);

static void InitManager();

ChatWin_Node* SearchManager(const char* ID); //查找对应窗口，存在返回其前驱，不存在返回最后一个结点

static void AddtoManager(ChatWin* chatwin, const char* ID);

int CreatNewChatWin(ChatWin* *p_chatwin, const char* ID) //建新窗口，失败返回0， 成功返回1，存在返回2
{
	ChatWin_Node* node = SearchManager(ID); //查询窗口是否存在
	if (node->next!=NULL) 
		return 2;

	ChatWin* chatwin = (ChatWin*) malloc(sizeof(ChatWin));
	chatwin->ID = (char*)malloc(sizeof(ID));
        strcpy(chatwin->ID, ID);
        chatwin->buffer_lock = 0;
	
	//加载builder
        GtkBuilder *builder;
        builder = gtk_builder_new ();
        if(gtk_builder_add_from_file (builder, "chat.xml", NULL)==0)
        {
  		free(chatwin);
                g_object_unref (G_OBJECT (builder));
                return 0;
        }
	//获取窗口
        chatwin->window = GTK_WIDGET (gtk_builder_get_object (builder, "window_chat"));
        if (chatwin->window==NULL)
        {
		free(chatwin);
                g_object_unref (G_OBJECT (builder));
                return 0;
        }
	//获取ID显示标签
        chatwin->label_ID = GTK_WIDGET (gtk_builder_get_object (builder, "label_ID"));
        if (chatwin->label_ID==NULL)
        {
		free(chatwin);
                g_object_unref (G_OBJECT (builder));
                return 0;
        }
	//获取文件名显示标签
        chatwin->label_file = GTK_WIDGET (gtk_builder_get_object (builder, "label_file"));
        if (chatwin->label_file==NULL)
        {
		free(chatwin);
                g_object_unref (G_OBJECT (builder));
                return 0;
        }	
	//获取输出文本视图
        chatwin->view_output = GTK_WIDGET (gtk_builder_get_object (builder, "textview_chat"));
        if (chatwin->view_output==NULL)
        {
                gdk_window_destroy (chatwin->window);
		free(chatwin);
                g_object_unref (G_OBJECT (builder));
                return 0;
        }
        //获取输出文本缓冲区
        chatwin->buffer_output = gtk_text_view_get_buffer(GTK_TEXT_VIEW(chatwin->view_output));
        if (chatwin->buffer_output==NULL)
        {
                gdk_window_destroy (chatwin->window);
                free(chatwin);
                g_object_unref (G_OBJECT (builder));
                return 0;
        }
        //获取输入文本视图
        chatwin->view_input = GTK_WIDGET (gtk_builder_get_object (builder, "textview_input"));
        if (chatwin->view_input==NULL)
        {
                gdk_window_destroy (chatwin->window);
		free(chatwin);
                g_object_unref (G_OBJECT (builder));
                return 0;
        }
        //获取输入文本缓冲区
        chatwin->buffer_input = gtk_text_view_get_buffer(GTK_TEXT_VIEW(chatwin->view_input));
        if (chatwin->buffer_input==NULL)
        {
                gdk_window_destroy (chatwin->window);
                free(chatwin);
                g_object_unref (G_OBJECT (builder));
                return 0;
        }
	//获取发送按钮
        chatwin->button_send = GTK_WIDGET (gtk_builder_get_object (builder, "button_send"));
        if (chatwin->button_send==NULL)
        {
                gdk_window_destroy (chatwin->window);
                free(chatwin);
                g_object_unref (G_OBJECT (builder));
                return 0;
        }
	//获取关闭按钮
	chatwin->button_close = GTK_WIDGET (gtk_builder_get_object (builder, "button_close"));
        if (chatwin->button_close==NULL)
        {
                gdk_window_destroy (chatwin->window);
                free(chatwin);
                g_object_unref (G_OBJECT (builder));
                return 0;
        }
	//获取文件发送按钮
	chatwin->button_sendfile = GTK_WIDGET (gtk_builder_get_object (builder, "button_sendfile"));
        if (chatwin->button_sendfile==NULL)
        {
                gdk_window_destroy (chatwin->window);
                free(chatwin);
                g_object_unref (G_OBJECT (builder));
                return 0;
        }
	//获取文件选择按钮
	chatwin->filechooserbutton = GTK_WIDGET (gtk_builder_get_object (builder, "filechooserbutton"));
        if (chatwin->filechooserbutton==NULL)
        {
                gdk_window_destroy (chatwin->window);
                free(chatwin);
                g_object_unref (G_OBJECT (builder));
                return 0;
        }
	//获取颜色选择按钮
	chatwin->colorbutton = GTK_WIDGET (gtk_builder_get_object (builder, "colorbutton"));
        if (chatwin->colorbutton==NULL)
        {
                gdk_window_destroy (chatwin->window);
                free(chatwin);
                g_object_unref (G_OBJECT (builder));
                return 0;
        }
	//获取字体选择按钮
	chatwin->fontbutton = GTK_WIDGET (gtk_builder_get_object (builder, "fontbutton"));
        if (chatwin->fontbutton==NULL)
        {
                gdk_window_destroy (chatwin->window);
                free(chatwin);
                g_object_unref (G_OBJECT (builder));
                return 0;
        }
	
        g_object_unref (G_OBJECT (builder));

	AddtoManager(chatwin, ID); //添加到管理器
	
	*p_chatwin = chatwin;

        return 1;
}

static void InitManager()
{
	if (manager!=NULL) return;
	manager = (ChatWin_Manager*)malloc(sizeof(ChatWin_Manager));
	manager->ID = NULL;
	manager->chatwin = NULL;
	manager->next = NULL;
}

ChatWin_Node* SearchManager(const char* ID)
{
	if (manager==NULL) 
	{
		InitManager();
		return manager;
	}

	ChatWin_Node* p = manager;
	for ( ; p->next!=NULL && strcmp(p->next->ID, ID)!=0; p=p->next ) ;

	return p;
}

static void AddtoManager(ChatWin* chatwin, const char* ID)
{
	ChatWin_Node* p = SearchManager(ID);
	p->next = (ChatWin_Node*)malloc(sizeof(ChatWin_Node));
	p = p->next;
	p->ID = (char*)malloc(sizeof(ID));
	strcpy(p->ID, ID);
	p->chatwin = chatwin;
	p->next = NULL;	
}

void DestoryChatWin(const char* ID)
{
	ChatWin_Node* p = SearchManager(ID);
	if (p->next==NULL) 
		return;
	
	ChatWin_Node* q = p->next;
	p->next = q->next;
	
	gtk_widget_destroy ((GdkWindow*)(q->chatwin->window));
	free(q->chatwin->ID);	
	free(q->chatwin);
	free(q->ID);
	free(q);
}

