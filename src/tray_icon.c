#include "tray_icon.h"

#include "mainloop.h"
#include "mixer_win.h"

#include <gtk/gtk.h>
#include <stdio.h>

static GtkStatusIcon * status_icon = 0;
static GtkWidget * menu = 0;

static gboolean icon_button_press(GtkStatusIcon * icon,
                              GdkEvent * ev,
                              gpointer ud) {

    GdkEventButton * evbtn = (GdkEventButton *) ev;

    switch (evbtn->button) {
    case 1:
        mixer_win_toggle();
        mixer_win_position(evbtn);
        return TRUE;
    case 3:
        gtk_menu_popup_at_pointer(GTK_MENU(menu), ev);
        return TRUE;

    default: return FALSE;
    }
}

static void menu_open_mixer(GtkWidget * widg, gpointer ud){
    mixer_win_show();
}

static void menu_quit(GtkWidget * widg, gpointer ud){
    mainloop_stop();
}

static void make_menu() {
    GtkWidget * itm = NULL;
    menu = gtk_menu_new();

    itm = gtk_menu_item_new_with_label("Open Mixer");
    g_signal_connect(itm, "activate", G_CALLBACK(menu_open_mixer), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), itm);

    itm = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), itm);

    itm = gtk_menu_item_new_with_label("Quit");
    g_signal_connect(itm, "activate", G_CALLBACK(menu_quit), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), itm);
    gtk_widget_show_all(menu);
}

void tray_icon_init() {
    status_icon = gtk_status_icon_new_from_icon_name(
        "audio-volume-high");

    make_menu();

    g_signal_connect(status_icon, "button-press-event",
                     G_CALLBACK(icon_button_press), NULL);
}
