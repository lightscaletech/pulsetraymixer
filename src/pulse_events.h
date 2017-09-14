#ifndef pulsetm_pulse_events_h_
#define pulsetm_pulse_events_h_

#include <stdint.h>
#include <pulse/def.h>
#include <pulse/context.h>
#include <pulse/introspect.h>

void pulse_events_cb(pa_context *, pa_subscription_event_type_t,
                     uint32_t, void *);

void pulse_events_sink_info(pa_context *, const pa_sink_info *,
                            int, void *);

void pulse_events_sink_input_info(pa_context *, const pa_sink_input_info *,
                                  int, void *);

#endif
