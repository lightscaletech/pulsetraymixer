#ifndef pulsetm_pulse_item_array_h_
#define pulsetm_pulse_item_array_h_

#include <stdint.h>

typedef struct _GList GList;
typedef GList PulseItemArray;

void * pulse_item_array_new();
void * pulse_item_array_remove(PulseItemArray **, uint32_t);
void * pulse_item_array_get(PulseItemArray *, uint32_t);
void pulse_item_array_add(PulseItemArray **, void *);

#endif
