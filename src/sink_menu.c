#include "sink_menu.h"
#include "pulse_control.h"

#include <stdlib.h>
#include <stdio.h>

struct ItemSelectData {
    int sink_input_idx;
    int sink_idx;
};

static gboolean item_selected(GtkRadioMenuItem * itm, GdkEvent * ev, void * ud) {
    struct ItemSelectData * d = (struct ItemSelectData *) ud;
    pulse_ctl_sink_input_move(d->sink_input_idx, d->sink_idx);
    return FALSE;
}

static void item_destroyed(GtkWidget * itm, void * ud) {
    free(ud);
}

static void menu_closed(GtkWidget * menu, void * ud) {
    gtk_widget_destroy(menu);
}

void sink_menu_create(MixerControlArray * sinkar, int si_idx, int current_sink,
                      GtkWidget * btn, GdkEvent * event) {
    GtkWidget * menu = gtk_menu_new();
    GtkWidget * menuItem = NULL;
    GSList * group = NULL;
    struct ItemSelectData * itmd = NULL;

    for (GList * i = sinkar; i != NULL; i = i->next) {
        MixerControl * ctl = i->data;
        char * name = mixer_control_get_label(ctl);
        menuItem = gtk_radio_menu_item_new_with_label(group, name);
        group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(menuItem));

        if(current_sink == ctl->pa_idx)
            gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuItem), TRUE);

        gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
        gtk_widget_show(menuItem);

        itmd = malloc(sizeof(struct ItemSelectData));
        itmd->sink_input_idx = si_idx;
        itmd->sink_idx = ctl->pa_idx;

        g_signal_connect(menuItem, "button-release-event",
                         G_CALLBACK(item_selected), itmd);
        g_signal_connect(menuItem, "destroy",
                         G_CALLBACK(item_destroyed), itmd);
    }

    g_signal_connect(menu, "selection-done", G_CALLBACK(menu_closed), NULL);

    gtk_menu_popup_at_widget(GTK_MENU(menu), btn,
                             GDK_GRAVITY_NORTH_WEST, GDK_GRAVITY_SOUTH_WEST,
                             event);
}
