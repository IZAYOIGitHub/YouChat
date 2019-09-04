#include <gtk/gtk.h>
#include <string.h>

void PopNotice(GtkWindow *parent, const char *title, const char *message)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(
        parent,
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_WARNING,
        GTK_BUTTONS_OK,
        message);
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

int PopConfirm(GtkWindow *parent, const char *title, const char *message)
{
    int dialogRespond = 0;
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(
        parent,
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_OK_CANCEL,
        message);
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    dialogRespond = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    if (dialogRespond == GTK_RESPONSE_OK || dialogRespond == GTK_RESPONSE_YES)
        return 1;
    else
        return 0;
}