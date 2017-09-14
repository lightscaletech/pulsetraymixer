#ifndef pulsetm_mixer_control_h_
#define pulsetm_mixer_control_h_

#include "pulse_item_array.h"

#include <stdint.h>

typedef struct _GtkWidget GtkWidget;
typedef int gint;
typedef gint gboolean;

struct _MixerControl {
    uint32_t pa_idx;
    GtkWidget * container;
    GtkWidget * icon;
    GtkWidget * slider;
    GtkWidget * label;
    GtkWidget * btnMute;
    GtkWidget * btnSettings;
};

typedef struct _MixerControl MixerControl;

MixerControl * mixer_control_new(uint32_t idx, const char * icon);
void mixer_control_free(MixerControl *);
void mixer_control_set_label(MixerControl *, const char *);
void mixer_control_set_muted(MixerControl *, gboolean);
void mixer_control_set_volume(MixerControl *, double);

typedef PulseItemArray MixerControlArray;

MixerControlArray * mixer_control_array_new();
void mixer_control_array_add(MixerControlArray **, MixerControl *);
MixerControl * mixer_control_array_remove(MixerControlArray **, uint32_t);
MixerControl * mixer_control_array_get(MixerControlArray *, uint32_t);

#endif
