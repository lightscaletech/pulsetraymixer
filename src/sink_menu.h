#ifndef pulsetm_sink_menu_h_
#define pulsetm_sink_menu_h_

#include "mixer_control.h"

#include <gtk/gtk.h>
#include <stdint.h>

void sink_menu_create(MixerControlArray *, int, int, GtkWidget *, GdkEvent *);

#endif
