#include "mixer_win.h"
#include "mixer_control.h"

#include "sink_menu.h"

#include "pulse_control.h"

#include <gtk/gtk.h>
#include <math.h>

static GtkWidget * win = NULL;
static GtkWidget * boxmain = NULL;
static GtkWidget * boxsink = NULL;
static GtkWidget * boxsource = NULL;

static MixerControlArray * mcasink = NULL;
static MixerControlArray * mcasource = NULL;

static gboolean focus_loss(GtkWidget * widg, GdkEvent * ev, gpointer ud){
    mixer_win_hide();
    return TRUE;
}

const GtkWidget * mixer_win_init() {
    GtkWidget * divi = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    boxmain = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    boxsink = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    boxsource = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    mcasink = mixer_control_array_new();
    mcasource = mixer_control_array_new();

    sink_menu_init();

    gtk_window_set_title(GTK_WINDOW(win), "Mixer");
    gtk_window_set_keep_above(GTK_WINDOW(win), TRUE);
    gtk_window_set_decorated(GTK_WINDOW(win), FALSE);

    gtk_container_add(GTK_CONTAINER(boxmain), boxsource);
    gtk_container_add(GTK_CONTAINER(boxmain), divi);
    gtk_container_add(GTK_CONTAINER(boxmain), boxsink);
    gtk_container_add(GTK_CONTAINER(win), boxmain);
    gtk_container_set_border_width(GTK_CONTAINER(win), 15);

    g_signal_connect(win, "delete-event",
                     G_CALLBACK(gtk_widget_hide_on_delete), NULL);

    //g_signal_connect(win, "focus-out-event", G_CALLBACK(focus_loss), NULL);

    return win;
}

void mixer_win_free() {
    gtk_widget_destroy(win);
    sink_menu_free();
}

void mixer_win_position(GdkEventButton * evbtn) {
    int evx = evbtn->x;
    int evy = evbtn->y;
    int winw, winh, winx, winy;
    GtkWindow * gwin = GTK_WINDOW(win);
    GdkDisplay * display = gdk_display_get_default();
    GdkMonitor * monitor = gdk_display_get_monitor_at_point(display, evx, evy);
    GdkRectangle monwa;

    gdk_monitor_get_workarea(monitor, &monwa);

    gtk_window_get_size(gwin, &winw, &winh);
    gtk_window_get_position(gwin, &winx, &winy);
    gtk_window_move(gwin,
                    (monwa.width - evx) - winw,
                    (monwa.height - evy) - winh);
}

static void resize() {
    GtkRequisition sink, source;
    GtkWindow * gwin = GTK_WINDOW(win);
    int winw, winh, winx, winy, neww, diffw;

    gtk_widget_get_preferred_size(boxsink, &sink, NULL);
    gtk_widget_get_preferred_size(boxsource, &source, NULL);

    gtk_window_get_size(gwin, &winw, &winh);
    gtk_window_get_position(gwin, &winx, &winy);

    neww = sink.width + source.width;
    diffw = round((winw - neww) / 2);

    gtk_window_resize(gwin, neww, winh);
    gtk_window_move(gwin, winx + diffw, winy);
}

void mixer_win_show() {
    gtk_widget_show_all(win);
}

void mixer_win_hide() {
    gtk_widget_hide(win);
}

void mixer_win_toggle() {
    if (gtk_widget_is_visible(win)) {
        mixer_win_hide();
    }
    else {
        mixer_win_show();
    }
}

GtkWidget * mixer_win_get() {
    return win;
}

static void update_control(MixerControl * ctl, const char * label,
                           gboolean muted, double vol) {
    mixer_control_set_label(ctl, label);
    mixer_control_set_muted(ctl, muted);
    mixer_control_set_volume(ctl, vol);
}

static void update_source_control(MixerControl * ctl, uint32_t sink_idx) {
    mixer_control_set_sink(ctl, sink_idx);
}

void mixer_win_source_add(uint32_t idx, uint32_t sink_idx,
                          const char * label, const char * icon,
                          gboolean muted, double vol){
    MixerControl * mc = mixer_control_new(idx, icon, TRUE);
    gtk_container_add(GTK_CONTAINER(boxsource), mc->container);
    gtk_widget_show_all(mc->container);
    mixer_control_set_mute_cb(mc, pulse_ctl_sink_input_mute);
    mixer_control_set_volume_cb(mc, pulse_ctl_sink_input_volume);
    mixer_control_set_menu(mc, sink_menu_get());
    mixer_control_array_add(&mcasource, mc);
    update_control(mc, label, muted, vol);
    update_source_control(mc, sink_idx);
}

void mixer_win_source_update(uint32_t idx, uint32_t sink_idx,
                             const char * label, gboolean muted, double vol) {
    MixerControl * mc = mixer_control_array_get(mcasource, idx);
    update_control(mc, label, muted, vol);
    update_source_control(mc, sink_idx);
}

void mixer_win_source_remove(uint32_t idx) {
    MixerControl * mc = mixer_control_array_remove(&mcasource, idx);
    if (mc == NULL) return;
    mixer_control_free(mc);
    resize();
}

void mixer_win_sink_add(uint32_t idx,
                        const char * label, const char * icon,
                        gboolean muted, double vol) {

    MixerControl * mc = mixer_control_new(idx, icon, FALSE);
    gtk_container_add(GTK_CONTAINER(boxsink), mc->container);
    gtk_widget_show_all(mc->container);
    mixer_control_array_add(&mcasink, mc);
    mixer_control_set_mute_cb(mc, pulse_ctl_sink_mute);
    mixer_control_set_volume_cb(mc, pulse_ctl_sink_volume);
    sink_menu_add(idx, label);
    update_control(mc, label, muted, vol);
}

void mixer_win_sink_update(uint32_t idx, const char * label,
                           gboolean muted, double vol) {
    MixerControl * mc = mixer_control_array_get(mcasink, idx);
    if (mc == NULL) return;
    update_control(mc, label, muted, vol);
}

void mixer_win_sink_remove(uint32_t idx) {
    MixerControl * mc = mixer_control_array_remove(&mcasink, idx);
    if (mc == NULL) return;
    sink_menu_remove(idx);
    mixer_control_free(mc);
    resize();
}
