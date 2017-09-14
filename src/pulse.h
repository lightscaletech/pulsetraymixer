#ifndef pulsetm_pulse_h_
#define pulsetm_pulse_h_

#include <pulse/context.h>

void pulse_init();
void pulse_iteration();
void pulse_cleanup();
pa_context * pulse_get_context();

#endif
