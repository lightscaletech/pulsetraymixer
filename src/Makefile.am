bin_PROGRAMS = pulsetraymixer

pulsetraymixer_CFLAGS = $(GTK_CFLAGS) $(PULSE_CFLAGS)
pulsetraymixer_LDADD = $(GTK_LIBS) $(PULSE_LIBS)
pulsetraymixer_SOURCES = mainloop.c mixer_control.c \
			mixer_win.c tray_icon.c sink_menu.c \
			pulse.c pulse_events.c pulse_item_array.c \
			pulse_event_block.c pulse_control.c main.c
