#include "pulse_events.h"
#include "mixer_win.h"

#include <pulse/pulseaudio.h>

#include <stdio.h>
#include <glib.h>

static const char * get_sink_name(const pa_sink_info * info) {
    pa_proplist * pl = info->proplist;
    return pa_proplist_gets(pl, PA_PROP_DEVICE_DESCRIPTION);
}

static const char * get_sink_icon_name(const pa_sink_info * info) {
    pa_proplist * pl = info->proplist;
    return pa_proplist_gets(pl, PA_PROP_DEVICE_ICON_NAME);
}

static void sink_new_info(pa_context * ctx, const pa_sink_info * info,
                          int eol, void * ud) {
    if(info == NULL) return;
    pa_volume_t vol = pa_cvolume_avg(&(info->volume));
    mixer_win_sink_add(info->index,
                       get_sink_name(info),
                       get_sink_icon_name(info),
                       info->mute, vol);
}

static void sink_change_info(pa_context * ctx, const pa_sink_info * info,
                             int eol, void * ud) {
    if(info == NULL) return;
    pa_volume_t vol = pa_cvolume_avg(&(info->volume));
    mixer_win_sink_update(info->index, get_sink_name(info), info->mute,
                          vol);
}

static void sink_new(pa_context * ctx, uint32_t idx) {
    pa_context_get_sink_info_by_index(ctx, idx, sink_new_info, NULL);
}

static void sink_change(pa_context * ctx, uint32_t idx) {
    pa_context_get_sink_info_by_index(ctx, idx, sink_change_info, NULL);
}

static void sink_remove(uint32_t idx) {
    mixer_win_sink_remove(idx);
}


static const char * get_source_name(const pa_sink_input_info * info) {
    pa_proplist * pl = info->proplist;
    const char * name = NULL;
    if((name = pa_proplist_gets(pl, PA_PROP_APPLICATION_NAME))) return name;
    if((name = pa_proplist_gets(pl, PA_PROP_MEDIA_NAME))) return name;
    return pa_proplist_gets(pl, PA_PROP_APPLICATION_PROCESS_BINARY);
}

static const char * get_source_icon_name(const pa_sink_input_info * info) {
    pa_proplist * pl = info->proplist;
    return pa_proplist_gets(pl, PA_PROP_APPLICATION_ICON_NAME);
}

static void source_new_info(pa_context * ctx, const pa_sink_input_info * info,
                            int eol, void * ud) {
    if(info == NULL) return;
    pa_volume_t vol = pa_cvolume_avg(&(info->volume));
    mixer_win_source_add(info->index, info->sink,
                         get_source_name(info),
                         get_source_icon_name(info),
                         info->mute, vol);
}

static void source_change_info(pa_context * ctx, const pa_sink_input_info * info,
                            int eol, void * ud) {
    if(info == NULL) return;
    pa_volume_t vol = pa_cvolume_avg(&(info->volume));
    mixer_win_source_update(info->index, info->sink, get_source_name(info), info->mute,
                          vol);
}

static void source_new(pa_context * ctx, uint32_t idx) {
    pa_context_get_sink_input_info(ctx, idx, source_new_info, NULL);
}

static void source_change(pa_context * ctx, uint32_t idx) {
    pa_context_get_sink_input_info(ctx, idx, source_change_info, NULL);
}

static void source_remove(uint32_t idx) {
    mixer_win_source_remove(idx);
}

static void sink_event(pa_context * ctx, uint type, uint32_t idx) {
    switch(type) {
    case PA_SUBSCRIPTION_EVENT_NEW: sink_new(ctx, idx); break;
    case PA_SUBSCRIPTION_EVENT_REMOVE: sink_remove(idx); break;
    case PA_SUBSCRIPTION_EVENT_CHANGE: sink_change(ctx, idx); break;
    }
}

static void source_event(pa_context * ctx, uint type, uint32_t idx) {
    switch(type) {
    case PA_SUBSCRIPTION_EVENT_NEW: source_new(ctx, idx); break;
    case PA_SUBSCRIPTION_EVENT_REMOVE: source_remove(idx); break;
    case PA_SUBSCRIPTION_EVENT_CHANGE: source_change(ctx, idx); break;
    }
}

void pulse_events_cb(pa_context * ctx, pa_subscription_event_type_t type,
                    uint32_t idx, void * ud) {
    uint fac = type & PA_SUBSCRIPTION_EVENT_FACILITY_MASK;
    uint op = type & PA_SUBSCRIPTION_EVENT_TYPE_MASK;

    switch(fac) {
    case PA_SUBSCRIPTION_EVENT_SINK_INPUT: source_event(ctx, op, idx); break;
    case PA_SUBSCRIPTION_EVENT_SINK: sink_event(ctx, op, idx); break;
    default: break;
    }
}


void pulse_events_sink_info(pa_context * ctx, const pa_sink_info * info,
                            int eol, void * ud) {
    sink_new_info(ctx, info, eol, ud);
}

void pulse_events_sink_input_info(pa_context * ctx,
                                  const pa_sink_input_info * info,
                              int eol, void * ud) {
    source_new_info(ctx, info, eol, ud);
}
