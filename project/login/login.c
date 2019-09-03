#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "cJSON.h"

static void button_login_clicked ();
static int CheckInput();
static void button_register_clicked();
int client_login(char* account,char* password);
int send_message_tcp(int send_socket_fd,char* buffer);

static GtkBuilder *builder_login;
static GtkWidget *window_login;
static GtkWidget *button_login;
static GtkWidget *button_register;
static GtkWidget* entry_username;
static GtkWidget* entry_password;

struct sockaddr_in server;

int main (int argc, char* argv[]) 
{ 
 	gtk_init(&argc,&argv);
	if(ShowLogin() != 0) exit(1);
	//gtk_widget_destory(window_login);
	//gtk_widget_hide(window_login);
	gtk_main ();

	return 0;

}


 	 
int ShowLogin() //显示登录界面
{
	builder_login = gtk_builder_new ();
        gtk_builder_add_from_file (builder_login, "login.xml", NULL); //把xml载入builder
	if(builder_login == NULL) return -1;
        window_login = GTK_WIDGET (gtk_builder_get_object (builder_login, "window_login")); //从builder获取构件
        if(window_login == NULL) return -1;
	button_login = GTK_WIDGET (gtk_builder_get_object (builder_login, "button_login"));
	if(button_login == NULL) return -1;
	button_register = GTK_WIDGET (gtk_builder_get_object (builder_login, "button_register"));
	if(button_register == NULL) return -1;
	entry_username = GTK_WIDGET (gtk_builder_get_object (builder_login, "entry_username"));
        if(entry_username == NULL) return -1;
	entry_password = GTK_WIDGET (gtk_builder_get_object (builder_login, "entry_password"));
        if(entry_password == NULL) return -1;

        g_object_unref (G_OBJECT (builder_login)); //释放xml使用的内存
	
	gtk_window_set_title(GTK_WINDOW(window_login),"登录"); //设置窗口标题
	g_signal_connect(G_OBJECT(window_login),"destroy", G_CALLBACK(gtk_main_quit),NULL); //为信号destroy绑定响应函数
        g_signal_connect(G_OBJECT(button_login),"clicked", G_CALLBACK(button_login_clicked),NULL);
	g_signal_connect(G_OBJECT(button_register),"clicked", G_CALLBACK(button_register_clicked),NULL);
        gtk_widget_show ((GtkWindow*)window_login); //显示窗口

	return 0;
}

static void button_login_clicked () //点击按钮时的响应函数
{
	const gchar *username = gtk_entry_get_text(GTK_ENTRY(entry_username)); 
	const gchar *password = gtk_entry_get_text(GTK_ENTRY(entry_password)); 

        if(CheckInput(username,password)==0)//检查输入
        {
		if( client_login((char*)username, (char*)password)==1)
		{
			g_print("success\n");
		}
		else
		{
			g_print("fail\n");
		}
		g_print("用户名是：%s  \n",username);
		g_print("密码是：%s  \n",password);
        }
        else//非法
        {
                //提示
        }
}

static int CheckInput(const gchar *username, const gchar *password)
{
	int len1,len2;
	len1=strlen(username);
	len2=strlen(password);
	if (len1 < 6 || len2 < 6) return -1;	

	return 0;
}
static void button_register_clicked()
{
	system("../register/register");
	return;
}


int client_login(char* account,char* password )
{
	int client_socket_fd;
	cJSON* json=cJSON_CreateObject();
	cJSON_AddNumberToObject(json,"type",1);
	cJSON_AddItemToObject(json,"username",cJSON_CreateString(account));
	cJSON_AddItemToObject(json,"password",cJSON_CreateString(password));
	
	char* buf = cJSON_Print(json);
	printf("datalogin:%s\n",buf);
	memset(&server, 0, sizeof(server));
		server.sin_family = AF_INET;
	// 	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_addr.s_addr = inet_addr("192.168.43.97");
		server.sin_port = htons(23333);
	socklen_t len;
	//     printf("hello\n");
		if( (client_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
		return 0;
		}
		if( connect(client_socket_fd, (struct sockaddr*)&server, sizeof(server)) < 0){
			printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
			return 0;
		}
	//发送账户
	//     printf("hello\n");
	send_message_tcp(client_socket_fd,buf);
	printf("send ok\n");
	//接收服务端返回信息
	recv(client_socket_fd,buf,1024,0);
	printf("recv data of my world is: %s\n",buf);
	if(strcmp(buf,"login_fail") == 0)
	{
		close(client_socket_fd);
		return 0;
	}
	else
		return 1;
	cJSON_Delete(json);
}

int send_message_tcp(int send_socket_fd,char* buffer){ // execute once, send message once

	if( send(send_socket_fd, buffer, strlen(buffer), 0) < 0){
		printf("send message error: %s(errno: %d)\n", strerror(errno), errno);
		return 0;
	}	
	return 1;
}