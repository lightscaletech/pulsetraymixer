#include "mainloop.h"
#include <glib.h>
#include <gtk/gtk.h>

static mainloop_cb loop_cb = NULL;
static mainloop_cb destroy_cb = NULL;

static GMainLoop * gloop = NULL;

static gboolean prepare(GSource * gs, gint * timeo) {
    if(gtk_events_pending()) return TRUE;
    *timeo = 50;
    return FALSE;
}

static gboolean check(GSource * gs) {
    return TRUE;
}

static gboolean dispatch(GSource * gs, GSourceFunc cb, gpointer ud) {
    if (cb) return cb(ud);
    return TRUE;
}

static gboolean loop(gpointer ud) {
    if (loop_cb) loop_cb();
    return TRUE;
}


void mainloop_run() {
    GMainContext * gctx = NULL;
    GSource * gsource = NULL;

    static GSourceFuncs gsfunc = {
        prepare,
        check,
        dispatch,
        NULL, NULL, NULL
    };

    gctx = g_main_context_new();
    gsource = g_source_new(&gsfunc, sizeof(GSource));
    g_source_set_callback(gsource, &loop, NULL, NULL);

    g_source_attach(gsource, gctx);

    gloop = g_main_loop_new(gctx, FALSE);

    // Run the loop
    g_main_loop_run(gloop);

    // Cleanup
    g_main_context_unref(gctx);
    g_source_unref(gsource);
    g_main_loop_unref(gloop);

    if(destroy_cb) destroy_cb();
}

void mainloop_stop() {
    g_main_loop_quit(gloop);
}

void mainloop_set_cb(mainloop_cb cb) {
    loop_cb = cb;
}

void mainloop_set_destroy_cb(mainloop_cb cb) {
    destroy_cb = cb;
}
