AC_PREREQ([2.69])
AC_INIT([libebus], [0.1], [roland.jax@liwest.at], [libebus], [https://github.com/yuhu-/libebus])

AM_INIT_AUTOMAKE([1.11 -Wall no-define])

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_HEADERS([config.h libebusconfig.h])

AC_PROG_CXX
AM_PROG_AR

LT_INIT([disable-static])

AC_SUBST([LIBEBUS_SO_VERSION], [0:0:0])
AC_SUBST([LIBEBUS_API_VERSION], [0.1])

AC_CONFIG_FILES([Makefile
		libebus-${LIBEBUS_API_VERSION}.pc:libebus.pc.in])
AC_OUTPUT
