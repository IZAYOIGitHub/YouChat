#ifndef MAINPAGE_H
#define MAINPAGE_H

/*
* 主界面，显示联系人(好友/群组)
* 主界面窗体定义在main.glide中
* 此处放置主界面相关的数据结构/函数定义
*/

#include <gtk/gtk.h>

/*--------------------常量定义------------------------*/

//联系人昵称的长度上限，计入\0
#define MAX_NICKNAME_LEN (64)


/*--------------------数据结构------------------------*/

/*--------------------函数声明------------------------*/

/*
* 打开主界面窗口，并加载联系人
* 返回指向新打开窗口的指针。如遇错误导致打开窗口失败，则返回NULL
*/
GtkWidget *OpenMainPage(void);

/*
* 刷新联系人列表
* 参数:
* name(gchararray)，联系人用户名/昵称
* message(gchararray)，该联系人发送来的消息
* 调用这个函数后，这个联系人会显示(若原有，则移动)到联系人列表的首端，同时会显示刚刚传来的消息
* 消息被截短，总长度不超过45字节。若原消息比45字节长，则加上"..."
*/
void UpdateContactList(gchararray name, gchararray message);

/*
* 开始一个新的聊天界面
* 此处没有启动新的线程
* 参数: tgtId(gchararray)，表示目标用户名/昵称
* 9/2: 还未对接聊天窗口，临时改为在控制台上打印相关信息
*/
void BeginTalk(gchararray tgtId);

/*
* 当联系人列表的一项被双击时，进入的函数。
* 找到目标ID，并开始一个聊天窗口。
* 若点选的是一个分类(如“群组”“好友”所在行)，则直接退出，不进行操作。
* 若点选的是“添加群组”“添加好友”所在行，则弹出加好友/加群窗口。
* 由gtk自行调用
* 9/3: 未对接加群/加好友窗口，临时改为在控制台上打印相关信息
*/
void OnContactActivate(GtkTreeView *view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer data);

#endif