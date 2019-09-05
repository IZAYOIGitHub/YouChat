#include "chatwin.h"
#include <gtk/gtk.h>

#ifndef CHAT_H
#define CHAT_H

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

void tcp_message_send(char* remote_account, char* message);

#endif
