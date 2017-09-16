#include "pulse_control.h"
#include "pulse_events.h"
#include "pulse.h"

#include <stdio.h>
#include <pulse/pulseaudio.h>

static void success(pa_context * ctx, int success, void * ud) {}

static pa_cvolume parse_volume(double vol) {
    pa_cvolume cvol;
    pa_volume_t vvol;
    if(vol < PA_VOLUME_MUTED) vvol = PA_VOLUME_MUTED;
    else if(vol > PA_VOLUME_NORM) vvol = PA_VOLUME_NORM;
    else vvol = vol;
    pa_cvolume_set(&cvol, 2, vvol);
    return cvol;
}

void pulse_ctl_sink_input_mute(uint32_t idx, int mute) {
    pa_context * ctx = pulse_get_context();
    pa_context_set_sink_input_mute(ctx, idx, mute, success, NULL);
}

void pulse_ctl_sink_input_volume(uint32_t idx, double vol) {
    pa_context * ctx = pulse_get_context();
    pa_cvolume cvol = parse_volume(vol);
    pa_context_set_sink_input_volume(ctx, idx, &cvol, success, NULL);
}

void pulse_ctl_sink_input_move(uint32_t idx, uint32_t sink_idx) {
    pa_context * ctx = pulse_get_context();
    pa_context_move_sink_input_by_index(ctx, idx, sink_idx, success, NULL);
}

void pulse_ctl_sink_mute(uint32_t idx, int mute) {
    pa_context * ctx = pulse_get_context();
    pa_context_set_sink_mute_by_index(ctx, idx, mute, success, NULL);
}

void pulse_ctl_sink_volume(uint32_t idx, double vol) {
    pa_context * ctx = pulse_get_context();
    pa_cvolume cvol = parse_volume(vol);
    pa_context_set_sink_volume_by_index(ctx, idx, &cvol, success, NULL);
}
