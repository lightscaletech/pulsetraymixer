#ifndef pulsetm_mixer_win_h_
#define pulsetm_mixer_win_h_

#include <stdint.h>

typedef struct _GtkApplication GtkApplication;
typedef struct _GtkWidget GtkWidget;
typedef struct _GdkEventButton GdkEventButton;
typedef int gint;
typedef gint gboolean;

const GtkWidget *  mixer_win_init();
GtkWidget *  mixer_win_get();

void mixer_win_show();
void mixer_win_hide();
void mixer_win_toggle();
void mixer_win_position(GdkEventButton *);

void mixer_win_source_add(uint32_t, uint32_t, const char *, const char *,
                          gboolean, double);
void mixer_win_source_update(uint32_t, uint32_t, const char *, gboolean, double);
void mixer_win_source_remove(uint32_t);

void mixer_win_sink_add(uint32_t, const char *,
                        const char *, gboolean, double);
void mixer_win_sink_update(uint32_t, const char *, gboolean, double);
void mixer_win_sink_remove(uint32_t);

#endif
