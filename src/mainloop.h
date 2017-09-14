#ifndef pulsetm_mainloop_h_
#define pulsetm_mainloop_h_

typedef void (* mainloop_cb)();

void mainloop_run();
void mainloop_stop();
void mainloop_set_cb(mainloop_cb);
void mainloop_set_destroy_cb(mainloop_cb);

#endif
