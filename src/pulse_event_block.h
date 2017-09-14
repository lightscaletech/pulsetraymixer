#ifndef pulsetm_pulse_event_block_h_
#define pulsetm_pulse_event_block_h_

#include "pulse_item_array.h"

#include <stdint.h>

struct PulseEvBlock {
    uint32_t pa_idx;
    int64_t time_set;
};

typedef struct PulseEvBlock PulseEvBlock;
typedef int gint;
typedef gint gboolean;

PulseEvBlock * pulse_event_block_new(uint32_t);
void pulse_event_block_free(PulseEvBlock *);
gboolean pulse_event_block_check(PulseEvBlock *);

typedef PulseItemArray PulseEvBlockArray;

PulseEvBlockArray * pulse_ev_block_array_new();
void pulse_ev_block_array_new_itm(PulseEvBlockArray **, uint32_t);
gboolean pulse_ev_block_array_check(PulseEvBlockArray **, uint32_t);

#endif
