#include "mixer_control.h"
#include "sink_menu.h"

#include <gtk/gtk.h>
#include <pulse/volume.h>

#include <stdlib.h>
#include <math.h>

static gboolean vol_change(GtkRange * r, GtkScrollType sc,
                       gdouble val, gpointer ud) {
    MixerControl * mc = (MixerControl *) ud;
    mixer_ctl_volume_cb cb = mc->vol_cb;
    if(cb == NULL) return FALSE;
    cb(mc->pa_idx, val);
    return FALSE;
}

static void mute_change(GtkToggleButton * tb, gpointer ud) {
    MixerControl * mc = (MixerControl *) ud;
    mixer_ctl_mute_cb cb = mc->mute_cb;
    if(cb == NULL) return;
    cb(mc->pa_idx, gtk_toggle_button_get_active(tb));
}

static gboolean settings_activate(GtkWidget * widg, GdkEvent * ev, void * ud) {
    MixerControl * mc = (MixerControl *) ud;
    mixer_control_menu_click_cb(mc->pa_idx, mc->sink_idx, widg, ev);
    return FALSE;
}

MixerControl * mixer_control_new(uint32_t idx, const char * icon, gboolean smenu) {
    MixerControl * mc = malloc(sizeof(MixerControl));
    double vol_max = PA_VOLUME_NORM;
    double vol_min = PA_VOLUME_MUTED;
    double vol_inc = (vol_max  - vol_min) / 20;

    // Extra containers
    GtkWidget * btnBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget * vBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget * hBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    // Set everything on struct
    mc->pa_idx = idx;
    mc->container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    mc->icon = gtk_image_new_from_icon_name(icon, GTK_ICON_SIZE_DND);
    mc->slider = gtk_scale_new_with_range(
        GTK_ORIENTATION_VERTICAL, vol_min, vol_max, vol_inc);
    mc->label = gtk_label_new("");

    mc->btnMute = gtk_toggle_button_new_with_label("M");
    if(smenu) {
        mc->btnSettings =
            gtk_button_new_from_icon_name("preferences-system", GTK_ICON_SIZE_MENU);
        g_signal_connect(mc->btnSettings, "clicked",
                         G_CALLBACK(settings_activate), mc);
    }
    else mc->btnSettings = NULL;

    // Configure the label
    gtk_label_set_angle(GTK_LABEL(mc->label), 90);

    // Configure slider
    gtk_widget_set_size_request(mc->slider, 20, 200);
    gtk_widget_set_margin_top(mc->slider, 15);
    gtk_scale_set_draw_value(GTK_SCALE(mc->slider), FALSE);
    gtk_range_set_inverted(GTK_RANGE(mc->slider), TRUE);

    // Connect events
    g_signal_connect(mc->slider, "change-value", G_CALLBACK(vol_change), mc);
    g_signal_connect(mc->btnMute, "toggled", G_CALLBACK(mute_change), mc);

    // Add everything to there containers.
    gtk_container_add(GTK_CONTAINER(btnBox), mc->btnMute);
    if (smenu) gtk_container_add(GTK_CONTAINER(btnBox), mc->btnSettings);
    gtk_container_add(GTK_CONTAINER(vBox), mc->icon);
    gtk_container_add(GTK_CONTAINER(vBox), mc->slider);
    gtk_container_add(GTK_CONTAINER(hBox), mc->label);
    gtk_container_add(GTK_CONTAINER(hBox), vBox);
    gtk_container_add(GTK_CONTAINER(mc->container), hBox);
    gtk_container_add(GTK_CONTAINER(mc->container), btnBox);

    return mc;
}

void mixer_control_free(MixerControl * mc) {
    gtk_widget_destroy(mc->container);
    free(mc);
}

void mixer_control_set_label(MixerControl * mc, const char * text){
    gtk_label_set_text(GTK_LABEL(mc->label), text);
}

char * mixer_control_get_label(MixerControl * mc) {
    gtk_label_get_text(GTK_LABEL(mc->label));
}

void mixer_control_set_muted(MixerControl * mc, gboolean mute){
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mc->btnMute), mute);
}

void mixer_control_set_volume(MixerControl * mc, double vol){
    gtk_range_set_value(GTK_RANGE(mc->slider), vol);
}

void mixer_control_set_mute_cb(MixerControl * mc, mixer_ctl_mute_cb cb) {
    mc->mute_cb = cb;
}

void mixer_control_set_volume_cb(MixerControl * mc, mixer_ctl_volume_cb cb) {
    mc->vol_cb = cb;
}

void mixer_control_set_sink(MixerControl * mc, uint32_t idx) {
    mc->sink_idx = idx;
}

void mixer_control_set_menu(MixerControl * mc, GtkWidget * menu) {

}

MixerControlArray * mixer_control_array_new() { return pulse_item_array_new(); }
MixerControl * mixer_control_array_remove(MixerControlArray ** ar, uint32_t k){
    return pulse_item_array_remove(ar, k);
}
MixerControl * mixer_control_array_get(MixerControlArray * ar , uint32_t k) {
    return pulse_item_array_get(ar, k);
}
void mixer_control_array_add(MixerControlArray ** ar, MixerControl * mc) {
    pulse_item_array_add(ar, mc);
}
