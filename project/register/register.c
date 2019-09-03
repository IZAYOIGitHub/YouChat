#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"
#include "common.h"

static void button_register_clicked ();
static int CheckInput();
int ShowRegister();

extern int client_register(char* account,char* password);

static GtkBuilder *builder_register;
static GtkWidget *window_register;
static GtkWidget *button_register;
static GtkWidget* entry_username;
static GtkWidget* entry_password;
static GtkWidget* entry_repeatpassword;

int main (int argc, char* argv[]) 
{ 
 	gtk_init(&argc,&argv);
	if(ShowRegister() != 0) exit(1);
	//gtk_widget_destory(window_register);
	//gtk_widget_hide(window_register);
g_print("Start\n");
	gtk_main ();

	return 0;

}



int ShowRegister() //显示注册界面
{
	builder_register = gtk_builder_new ();
        gtk_builder_add_from_file (builder_register, "register.xml", NULL); //把xml载入builder
	if(builder_register == NULL) return -1;
        window_register = GTK_WIDGET (gtk_builder_get_object (builder_register, "window_register")); //从builder获取构件
        if(window_register == NULL) return -1;
	button_register = GTK_WIDGET (gtk_builder_get_object (builder_register, "button_register"));
	if(button_register == NULL) return -1;
	entry_username = GTK_WIDGET (gtk_builder_get_object (builder_register, "entry_username"));
        if(entry_username == NULL) return -1;
	entry_password = GTK_WIDGET (gtk_builder_get_object (builder_register, "entry_password"));
        if(entry_password == NULL) return -1;
 	entry_repeatpassword = GTK_WIDGET (gtk_builder_get_object (builder_register, "entry_repeatpassword"));
	if(entry_repeatpassword == NULL) return -1;

        g_object_unref (G_OBJECT (builder_register)); //释放xml使用的内存
	
	gtk_window_set_title(GTK_WINDOW(window_register),"注册"); //设置窗口标题
	g_signal_connect(G_OBJECT(window_register),"destroy", G_CALLBACK(gtk_main_quit),NULL); //为信号destroy绑定响应函数
        g_signal_connect(G_OBJECT(button_register),"clicked", G_CALLBACK(button_register_clicked),NULL);
        gtk_widget_show ((GtkWidget*)window_register); //显示窗口

	return 0;
}

static void button_register_clicked () //点击按钮时的响应函数
{
	gchar *username = gtk_entry_get_text(GTK_ENTRY(entry_username)); 
	gchar *password = gtk_entry_get_text(GTK_ENTRY(entry_password)); 
	gchar *repeatpassword = gtk_entry_get_text(GTK_ENTRY(entry_repeatpassword)); 
        if(CheckInput(username,password,repeatpassword)==0)//检查输入
        {
               if( client_register((char*)username, (char*)password)==1)
		{
			g_print("成功");//注册成功
		}
		else
		{
			g_print("失败");//注册失败
		}
		g_print("用户名是：%s  \n",username);
		g_print("密码是：%s  \n",password);

        }
        else//非法
        {
		g_print("error  \n");
                //提示
        }
}

static int CheckInput(const gchar *username, const gchar *password, const gchar *repeatpassword)
{
	int len1,len2;
	len1=strlen(username);
	len2=strlen(password);
	if(strcmp(password,repeatpassword)!=0) return -1;
	if (len1 < 6 || len2 < 6) return -1;	

	return 0;
}

