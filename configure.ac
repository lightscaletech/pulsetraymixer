AC_INIT([pulsetraymixer], [0.0.1], [sam@lightscale.co.uk])
AM_INIT_AUTOMAKE
AC_PROG_CC

AC_CONFIG_MACRO_DIR([m4])

PKG_CHECK_MODULES([GTK], [gtk+-3.0])
PKG_CHECK_MODULES([PULSE], [libpulse])

AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([
    Makefile
    src/Makefile
])

AC_OUTPUT
