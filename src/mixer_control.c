#include "mixer_control.h"

#include <gtk/gtk.h>
#include <pulse/volume.h>

#include <stdlib.h>
#include <math.h>

MixerControl * mixer_control_new(uint32_t idx, const char * icon) {
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
    mc->btnSettings = gtk_button_new_with_label("S");

    // Configure the label
    gtk_label_set_angle(GTK_LABEL(mc->label), 90);

    // Configure slider
    gtk_widget_set_size_request(mc->slider, 20, 200);
    gtk_widget_set_margin_top(mc->slider, 15);
    gtk_scale_set_draw_value(GTK_SCALE(mc->slider), FALSE);
    gtk_range_set_inverted(GTK_RANGE(mc->slider), TRUE);

    // Add everything to there containers.
    gtk_container_add(GTK_CONTAINER(btnBox), mc->btnMute);
    gtk_container_add(GTK_CONTAINER(btnBox), mc->btnSettings);
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

void mixer_control_set_muted(MixerControl * mc, gboolean mute){
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mc->btnMute), mute);
}

void mixer_control_set_volume(MixerControl * mc, double vol){
    gtk_range_set_value(GTK_RANGE(mc->slider), vol);
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
