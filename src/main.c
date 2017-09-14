#include "../config.h"

#include "mixer_win.h"
#include "tray_icon.h"
#include "mainloop.h"
#include "pulse.h"


#include <gtk/gtk.h>
#include <stdio.h>

static void app_activate() {
    pulse_init();
    mixer_win_init();
    tray_icon_init();
}

static void main_loop() {
    gtk_main_iteration_do(FALSE);
    pulse_iteration();
}

static void cleanup() {
    pulse_cleanup();
}

int main(int argc, char **argv) {
    int status = 0;

    puts (PACKAGE_NAME " - " PACKAGE_VERSION);

    gtk_init(&argc, &argv);
    app_activate();

    mainloop_set_cb(main_loop);
    mainloop_set_destroy_cb(cleanup);
    mainloop_run();

    return status;
}
