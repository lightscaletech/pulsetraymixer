#ifndef pulsetm_sink_menu_h_
#define pulsetm_sink_menu_h_

#include <stdint.h>

typedef struct _GtkWidget GtkWidget;

void sink_menu_init();
void sink_menu_free();
GtkWidget * sink_menu_get();
void sink_menu_add(uint32_t, const char *);
void sink_menu_remove(uint32_t);
void sink_menu_set_sink_input(uint32_t);
void sink_menu_set_selected(uint32_t);

#endif
