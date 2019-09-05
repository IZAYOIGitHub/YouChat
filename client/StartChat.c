#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

int ShowWindow();
static int CheckInput(const gchar *id);
static void button_StartChat_clicked();
int OpenChatWin(const char *tgtId);

static GtkBuilder *builder;
static GtkWidget *window;
static GtkWidget *button_start;
static GtkWidget *entry_userid;

static void button_start_clicked() //点击按钮时的响应函数
{
	const gchar *userid = gtk_entry_get_text(GTK_ENTRY(entry_userid));

	if (CheckInput(userid) == 0) //检查输入
	{
		//发回后端
		//有该用户开始聊天
		//没有则弹窗提示
		g_print("用户名是：%s  \n", userid);
		OpenChatWin(userid);
		gtk_widget_destroy(window);
	}
	else //非法
	{
		//提示
	}
}

static int CheckInput(const gchar *id)
{
	int len1;
	len1 = strlen(id);
	if (len1 < 1)
		return -1;

	return 0;
}

int ShowWindow()
{
	// 创建GtkBuilder对象, GtkBuilder在<gtk/gtk.h>声明
	builder = gtk_builder_new();

	// 读取test.glade文件的信息，保存在builder指针变量里，返回值不为空代表读取成功
	if (!gtk_builder_add_from_file(builder, "./StartChat.glade", NULL))
	{
		printf("cannot load file!");
	}

	// 获取窗口控件指针
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_start"));

	entry_userid = GTK_WIDGET(gtk_builder_get_object(builder, "entry_userid"));
	button_start = GTK_WIDGET(gtk_builder_get_object(builder, "button_start"));

	g_object_unref(G_OBJECT(builder)); //释放xml使用的内存

	gtk_window_set_title(GTK_WINDOW(window), "开始聊天"); //设置窗口标题

	g_signal_connect(button_start, "clicked", G_CALLBACK(button_start_clicked), NULL);

	gtk_widget_show_all(window);
	return 0;
}
/*
int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	if(ShowWindow() != 0) exit(1);
//g_print("start\n");

	gtk_main();
 
    return 0;
}
*/