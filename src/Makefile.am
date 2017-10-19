bin_PROGRAMS = pulsetraymixer

pulsetraymixer_CFLAGS = $(GTK_CFLAGS) $(PULSE_CFLAGS)
pulsetraymixer_LDADD = $(GTK_LIBS) $(PULSE_LIBS)
pulsetraymixer_SOURCES = \
	mainloop.c mixer_control.c \
	mixer_win.c tray_icon.c sink_menu.c \
	pulse.c pulse_events.c pulse_item_array.c \
	pulse_event_block.c pulse_control.c main.c \
	resources.c

resource_files = $(shell glib-compile-resources --sourcedir=$(srcdir) --generate-dependencies $(srcdir)/pulsetraymixer.gresources.xml)

resources.h: pulsetraymixer.gresources.xml $(resource_files)
	$(AM_V_GEN) glib-compile-resources --target $@ \
		--sourcedir=$(srcdir) --generate-header \
		--c-name=pulsetraymixer $<

resources.c: pulsetraymixer.gresources.xml resources.h $(resource_files)
	$(AM_V_GEN) glib-compile-resources --target $@ \
		--sourcedir=$(srcdir) --generate-source \
		--c-name=pulsetraymixer $<


clean-local:
	rm -f resources.h resources.c
