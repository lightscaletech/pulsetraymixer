#include "sink_menu.h"
#include "pulse_item_array.h"
#include "pulse_control.h"

#include <stdlib.h>
#include <gtk/gtk.h>

#include <stdio.h>

static GtkWidget * menu = NULL;
static PulseItemArray * items = NULL;
static uint32_t sink_input = 0;
static GSList * radio_group = NULL;

struct MenuLink {
    uint32_t pa_idx;
    GtkWidget * item;
};

void sink_menu_init() {
    menu = gtk_menu_new();
    items = pulse_item_array_new();
}

void sink_menu_free() {
    sink_input = 0;
    gtk_widget_destroy(menu);
}

GtkWidget * sink_menu_get() {
    return menu;
}

static gboolean item_selected(GtkRadioMenuItem * itm, GdkEvent * ev, void * ud) {
    uint32_t idx = *((uint32_t *) ud);
    pulse_ctl_sink_input_move(sink_input, idx);
    return FALSE;
}

void sink_menu_add(uint32_t idx, const char * name) {
    struct MenuLink * link = malloc(sizeof(struct MenuLink));
    GtkWidget * itm = gtk_radio_menu_item_new_with_label(
        radio_group, name);
    radio_group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(itm));

    link->pa_idx = idx;
    link->item = itm;

    g_signal_connect(itm, "button-release-event",
                     G_CALLBACK(item_selected), &link->pa_idx);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), itm);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(itm), FALSE);
    gtk_widget_show(itm);
    pulse_item_array_add(&items, link);
}

void sink_menu_remove(uint32_t idx) {
    struct MenuLink * link =
        (struct MenuLink *) pulse_item_array_remove(&items, idx);
    gtk_widget_destroy(link->item);
    free(link);
}

void sink_menu_set_sink_input(uint32_t idx) {
    sink_input = idx;
}

void sink_menu_set_selected(uint32_t idx) {
    struct MenuLink * link =
        (struct MenuLink *) pulse_item_array_get(items, idx);
    if(link == NULL) return;
    GtkWidget * item = link->item;
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), TRUE);
}
