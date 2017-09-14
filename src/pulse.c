#include "pulse.h"
#include "pulse_events.h"

#include "mainloop.h"

#include "../config.h"

#include <stdio.h>
#include <pulse/pulseaudio.h>

static pa_mainloop * mloop = NULL;
static pa_context * ctx = NULL;

static void subscribe_success(pa_context * ctx, int suc, void * ud) {}

static void context_ready() {
    puts("READY");
    pa_context_set_subscribe_callback(ctx, pulse_events_cb, NULL);
    pa_context_subscribe(ctx,
                         PA_SUBSCRIPTION_MASK_SINK |
                         PA_SUBSCRIPTION_MASK_SINK_INPUT,
                         subscribe_success, NULL);
    pa_context_get_sink_info_list(ctx, pulse_events_sink_info, NULL);
    pa_context_get_sink_input_info_list(ctx, pulse_events_sink_input_info, NULL);
}

static void state_cb(pa_context * ctx, void * ud) {
    pa_context_state_t state = pa_context_get_state(ctx);
    switch(state) {
    case PA_CONTEXT_UNCONNECTED: puts("UNCONNECTED"); break;
    case PA_CONTEXT_CONNECTING: puts("CONNECTING"); break;
    case PA_CONTEXT_AUTHORIZING: puts("AUTHORIZING"); break;
    case PA_CONTEXT_SETTING_NAME: puts("SETTING_NAME"); break;
    case PA_CONTEXT_READY: context_ready(); break;
    case PA_CONTEXT_FAILED: mainloop_stop(); break;
    case PA_CONTEXT_TERMINATED: break;
    }
}

void pulse_init() {
    pa_mainloop_api * mlapi;
    mloop = pa_mainloop_new();
    mlapi = pa_mainloop_get_api(mloop);
    ctx = pa_context_new(mlapi, PACKAGE_NAME);
    pa_context_connect(ctx, NULL, PA_CONTEXT_NOFLAGS, NULL);

    pa_context_set_state_callback(ctx, state_cb, NULL);
}

void pulse_iteration() {
    int rv = 0;
    pa_mainloop_iterate(mloop, 0, &rv);
}

void pulse_cleanup() {
    pa_context_disconnect(ctx);
    pa_context_unref(ctx);
    pa_mainloop_free(mloop);
}
