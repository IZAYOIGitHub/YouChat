#ifndef POPUP_H
#define POPUP_H

/*
* popup.h
* 封装了弹出对话框用的函数
*/


#include <gtk/gtk.h>
#include <string.h>

/*
*弹出对话框,具有一个"OK＂按钮
*参数:
*parent 弹出对话框的母窗体指针,可以为空　GtkWindow*
*title 对话框标题 const char*
*message 对话框内显示的文字 const char* 
*/
void PopNotice(GtkWindow *parent, const char *title, const char *message);

/*
* 弹出对话框，具有一个"OK"按钮和一个"cancel"按钮
* 参数:
*parent 弹出对话框的母窗体,可以为空　GtkWindow*
*title 对话框标题 const char*
*message 对话框内显示的文字 const char* 
* 返回值:
* 若点击OK,则返回1;否则返回0
*/
int PopConfirm(GtkWindow *parent, const char *title, const char *message);

#endif