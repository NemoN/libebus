AC_PREREQ([2.65])
AC_INIT([libebus], [0.2.0], [roland.jax@liwest.at], [libebus], [https://github.com/yuhu-/libebus])

AC_CONFIG_SRCDIR([libebus/bus.cpp])
AC_CONFIG_AUX_DIR([build])
AM_INIT_AUTOMAKE([1.11 -Wall -Werror foreign])

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_HEADERS([config.h libebusconfig.h])

AC_PROG_CXX
AM_PROG_AR

LT_INIT([disable-static])

AC_SUBST([LIBEBUS_SO_VERSION], [0:2:0])
AC_SUBST([LIBEBUS_API_VERSION], [0.2])

AC_CONFIG_FILES([Makefile
		libebus-${LIBEBUS_API_VERSION}.pc:libebus.pc.in])
AC_OUTPUT
