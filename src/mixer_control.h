#ifndef pulsetm_mixer_control_h_
#define pulsetm_mixer_control_h_

#include "pulse_item_array.h"

#include <gdk/gdk.h>
#include <stdint.h>

typedef int gint;
typedef gint gboolean;
typedef struct _GtkWidget GtkWidget;

typedef void(* mixer_ctl_mute_cb)(uint32_t, int);
typedef void(* mixer_ctl_volume_cb)(uint32_t, double);

typedef void(* menu_btn_click)(int, int, GtkWidget *, GdkEvent *);

menu_btn_click mixer_control_menu_click_cb;

struct _MixerControl {
    uint32_t pa_idx;
    uint32_t sink_idx;
    GtkWidget * container;
    GtkWidget * icon;
    GtkWidget * slider;
    GtkWidget * label;
    GtkWidget * btnMute;
    GtkWidget * btnSettings;

    mixer_ctl_mute_cb mute_cb;
    mixer_ctl_volume_cb vol_cb;
};

typedef struct _MixerControl MixerControl;

MixerControl * mixer_control_new(uint32_t idx, const char * icon, gboolean);
void mixer_control_free(MixerControl *);
void mixer_control_set_label(MixerControl *, const char *);
char * mixer_control_get_label(MixerControl *);
void mixer_control_set_muted(MixerControl *, gboolean);
void mixer_control_set_mute_cb(MixerControl *, mixer_ctl_mute_cb);
void mixer_control_set_volume(MixerControl *, double);
void mixer_control_set_volume_cb(MixerControl *, mixer_ctl_volume_cb);
void mixer_control_set_menu(MixerControl *, GtkWidget *);
void mixer_control_set_sink(MixerControl *, uint32_t);

typedef PulseItemArray MixerControlArray;

MixerControlArray * mixer_control_array_new();
void mixer_control_array_add(MixerControlArray **, MixerControl *);
MixerControl * mixer_control_array_remove(MixerControlArray **, uint32_t);
MixerControl * mixer_control_array_get(MixerControlArray *, uint32_t);

#endif
