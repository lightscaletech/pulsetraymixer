#include "pulse_event_block.h"

#include <stdlib.h>
#include <glib.h>

static const int64_t TIMEOUT = 1000 * 500;

PulseEvBlock * pulse_event_block_new(uint32_t idx) {
    PulseEvBlock * eb = malloc(sizeof(PulseEvBlock));
    eb->pa_idx = idx;
    eb->time_set = g_get_monotonic_time();
    return eb;
}

void pulse_event_block_free(PulseEvBlock * eb) {
    free(eb);
}

gboolean pulse_event_block_check(PulseEvBlock * eb) {
    return (eb->time_set + TIMEOUT) < g_get_monotonic_time();
}

PulseEvBlockArray * pulse_ev_block_array_new() {
    return pulse_item_array_new();
}

static void refresh_time(PulseEvBlock * eb) {
    eb->time_set = g_get_monotonic_time();
}

void pulse_ev_block_array_new_itm(PulseEvBlockArray ** ar, uint32_t idx) {
    PulseEvBlock * eb = pulse_item_array_get(*ar, idx);

    if(eb == NULL) eb = pulse_event_block_new(idx);
    else refresh_time(eb);

    pulse_event_block_new(idx);
    pulse_item_array_add(ar, eb);
}

gboolean pulse_ev_block_array_check(PulseEvBlockArray ** ar, uint32_t idx) {
    PulseEvBlock * eb = pulse_item_array_get(*ar, idx);

    if (pulse_event_block_check(eb)) {
        pulse_event_block_free(eb);
        pulse_item_array_remove(ar, idx);
        return TRUE;
    }
    else return FALSE;
}
