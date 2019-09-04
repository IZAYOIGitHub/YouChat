/*
* mainPage.c
* 主界面，显示联系人(好友/群组)
* 主界面窗体定义在main.glide中
* 双击联系人列表中的一项，即可打开聊天窗口开始聊天。
* 9/2: 还未对接聊天窗口，临时改为在控制台上打印相关信息
* 9/2: 还未对接后端，临时采用硬编码的假数据
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mainPage.h"

GtkWidget *contactListView = NULL;
GtkTreeStore *contactListStore = NULL;

#pragma region 对接接口函数
//临时声明的函数，由其他人实现
//启动监听新消息线程
void create_receive_thread(void)
{
    g_print("create_receive_thread()\n");
    return;
}

#ifndef CHAT_H
//临时声明的函数，由其他人实现
//打开聊天窗口
int OpenChatWin(const char *tgtId)
{
    printf("Begining talk with ID %s\n", tgtId);
    return 0;
}
#endif

//临时声明的函数，由其他人实现
//打开“新聊天”对话框
int ShowWindow(void)
{
    g_print("Opening window to create a new chat\n");
    return 0;
}

#pragma endregion 这些函数由别人实现，这里暂时用打印相关信息代替

void OnContactActivate(GtkTreeView *view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer data)
{
    GtkTreeIter iter, parent;
    GtkTreeModel *model = gtk_tree_view_get_model(view);
    gtk_tree_model_get_iter(model, &iter, path);
    gchararray tgtId = NULL;
    gchararray name = NULL;
    gtk_tree_model_get(model, &iter, 0, &tgtId, -1);
    BeginTalk(tgtId);
}

void OnNewChatClicked(GtkWidget *widget, gpointer data)
{
    if (ShowWindow()) //打开新聊天窗口失败
        g_error("Critical: Failed to open new chat wizard\n");
    return;
}

GtkWidget *OpenMainPage(void)
{
    //准备窗口
    gchararray fileName = "main.glade", windowName = "window1";
    GtkBuilder *builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, fileName, NULL))
    {
        g_error("Fatal: Failed to open resource file main.glade\n");
        g_object_unref(builder);
        return NULL;
    }
    GtkWidget *window;
    window = GTK_WIDGET(gtk_builder_get_object(builder, windowName));
    if (!window)
    {
        g_error("Fatal: Failed to get window from main.glade\n");
        g_object_unref(builder);
        return NULL;
    }

    contactListStore = GTK_TREE_STORE(gtk_builder_get_object(builder, "contactStore"));
    contactListView = GTK_WIDGET(gtk_builder_get_object(builder, "contactList"));
    gtk_builder_connect_signals(builder, NULL);
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("昵称", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(contactListView), column);
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(contactListView), column);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(contactListView), "row-activated", G_CALLBACK(OnContactActivate), NULL);

    GObject *btnNewChat;
    btnNewChat = gtk_builder_get_object(builder, "buttonNewChat");
    g_signal_connect(btnNewChat, "clicked", G_CALLBACK(OnNewChatClicked), NULL);

    create_receive_thread();
    gtk_widget_show_all(window);
    return window;
}

void UpdateContactList(gchararray name, gchararray message)
{
    //把消息字符串截断，保持UTF-8宽字符完整
    gchararray msgCut = g_malloc0(sizeof(gchar) * 50);
    g_strlcpy(msgCut, message, 50);
    if (strlen(msgCut) >= 45) //原消息长于45字节，则截短并加入"..."
    {
        int cutPos;
        for (cutPos = 45; cutPos; cutPos--)
            if ((msgCut[cutPos] >> 7 == 0) /*is an ASCII*/ || msgCut[cutPos] >> 6 == 0b11 /*first byte of UTF-8*/)
                break;
        msgCut[cutPos] = '\0';
        g_strlcat(msgCut, "...", 50);
    }

    //将截短后消息中的回车和tab变成空格
    for (int i = 0; i < 45; i++)
        if (msgCut[i] == '\n' || msgCut[i] == '\t' || msgCut[i] == '\r')
            msgCut[i] = ' ';

    //在列表中加入新项，并删除所有用户名于发来消息的用户相同的项
    GtkTreeIter iter;
    gchararray nameBuf = NULL; //储存列表中取出的名称，用于对比
    //nameBuf = g_malloc0(MAX_NICKNAME_LEN * sizeof(gchar));
    gtk_tree_store_insert(contactListStore, &iter, NULL, 0);
    gtk_tree_store_set(contactListStore, &iter, 0, name, 1, msgCut, -1);
    while (gtk_tree_model_iter_next(gtk_tree_view_get_model(GTK_TREE_VIEW(contactListView)), &iter))
    {
        gtk_tree_model_get(gtk_tree_view_get_model(GTK_TREE_VIEW(contactListView)), &iter, 0, &nameBuf, -1);
        if (!g_strcmp0(nameBuf, name))                           //ID相同的联系人，只保留上方一个
            if (!gtk_tree_store_remove(contactListStore, &iter)) //删除后iter不再有效，即之前被删除的行是最后一行
                break;
    }
    return;
}

void BeginTalk(gchararray tgtId)
{

    OpenChatWin(tgtId);
    return;
}
