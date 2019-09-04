#include "mainPage.h"

gint handler = 0;
GtkWidget *entryName, *viewMsg;

void cb_SendMsg(GtkWidget *widget, gpointer data)
{
    GtkTextIter begin, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(viewMsg));
    gtk_text_buffer_get_bounds(buffer, &begin, &end);
    gchararray name, msg;
    name = gtk_entry_get_text(GTK_ENTRY(entryName));
    msg = gtk_text_buffer_get_text(buffer, &begin, &end, TRUE);
    g_print("%s:\n%s\n", name, msg);
    UpdateContactList(name, msg);
    return;
}

int main(int argc, char *argv[])
{
    GtkWidget *window, *button, *mainPage;
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "主界面测试");
    handler = g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    mainPage = OpenMainPage();

    GtkWidget *vbox;
    vbox = gtk_vbox_new(FALSE, FALSE);
    entryName = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entryName), 60);
    gtk_box_pack_start(GTK_BOX(vbox), entryName, FALSE, FALSE, 10);

    viewMsg = gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(vbox), viewMsg, FALSE, FALSE, 10);

    button = gtk_button_new_with_label("发送");
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 10);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(cb_SendMsg), NULL);

    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
