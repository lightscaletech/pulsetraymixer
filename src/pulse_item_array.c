#include "pulse_item_array.h"
#include <glib.h>

struct PulseItem {
    uint32_t pa_idx;
};

static PulseItemArray * find_item(PulseItemArray * ar, uint32_t k) {
    PulseItemArray * l;
    struct PulseItem * d;
    for (l = ar; l != NULL; l = l->next) {
        d = (struct PulseItem *) l->data;
        if(d != NULL && d->pa_idx == k) return l;
    }
    return NULL;
}

void * pulse_item_array_new() { return NULL; }

void * pulse_item_array_remove(PulseItemArray ** ar, uint32_t k) {
    PulseItemArray * item = find_item(*ar, k);

    if (item == NULL) return NULL;

    struct PulseItem * d = item->data;
    *ar = g_list_remove(*ar, d);
    return d;
}

void * pulse_item_array_get(PulseItemArray * ar, uint32_t k) {
    PulseItemArray * item = find_item(ar, k);
    if(item == NULL) return NULL;
    return item->data;
}

void pulse_item_array_add(PulseItemArray ** ar, void * i) {
    *ar = g_list_prepend(*ar, i);
}
