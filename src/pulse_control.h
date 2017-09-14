#ifndef pulsetm_pulse_control_h_
#define pulsetm_pulse_control_h_

#include <stdint.h>

typedef uint32_t pa_volume_t;

void pulse_ctl_sink_input_mute(uint32_t, int);
void pulse_ctl_sink_input_volume(uint32_t, double);
void pulse_ctl_sink_mute(uint32_t, int);
void pulse_ctl_sink_volume(uint32_t, double);

#endif
