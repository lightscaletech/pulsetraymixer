AC_INIT([pulsetraymixer], [0.0.1], [sam@lightscale.co.uk])
AM_INIT_AUTOMAKE
AC_PROG_CC

AC_CONFIG_MACRO_DIR([m4])

PKG_CHECK_MODULES([GTK], [gtk+-3.0])
PKG_CHECK_MODULES([PULSE], [libpulse])

AC_CHECK_PROG(HAVE_GRESOURCES_CC, glib-compile-resources, true, false)
if(test "x$HAVE_GRESOURCES_CC" = "xfalse"); then
        AC_MSG_ERROR([Failed to find glib-compile-resources.])
fi

AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([
    Makefile
    src/Makefile
])

AC_OUTPUT
