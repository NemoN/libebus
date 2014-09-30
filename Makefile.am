AUTOMAKE_OPTIONS = subdir-objects

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

AM_CXXFLAGS = -fpic -Wall -Wextra

lib_LTLIBRARIES = libebus-@LIBEBUS_API_VERSION@.la

libebus_@LIBEBUS_API_VERSION@_la_SOURCES = libebus/port.cpp \
					   libebus/bus.cpp \
					   libebus/commands.cpp \
					   libebus/config.cpp \
					   libebus/dump.cpp \
					   libebus/decode.cpp \
					   libebus/encode.cpp

libebus_@LIBEBUS_API_VERSION@_la_LDFLAGS = -version-info $(LIBEBUS_SO_VERSION)

libebus_includedir = $(includedir)/libebus-$(LIBEBUS_API_VERSION)
nobase_libebus_include_HEADERS = libebus.h \
                                 libebus/port.h \
                                 libebus/bus.h \
                                 libebus/commands.h \
                                 libebus/config.h \
                                 libebus/dump.h \
                                 libebus/decode.h \
                                 libebus/encode.h

libebus_libincludedir = $(libdir)/libebus-$(LIBEBUS_API_VERSION)/include
nodist_libebus_libinclude_HEADERS = libebusconfig.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libebus-$(LIBEBUS_API_VERSION).pc

dist_noinst_SCRIPTS = autogen.sh


noinst_PROGRAMS = test_port \
		  test_bus \
		  test_commands \
		  test_config \
		  test_decode \
		  test_encode

test_port_SOURCES = test/test_port.cpp
test_port_LDADD = libebus-$(LIBEBUS_API_VERSION).la

test_bus_SOURCES = test/test_bus.cpp
test_bus_LDADD = libebus-$(LIBEBUS_API_VERSION).la

test_commands_SOURCES = test/test_commands.cpp
test_commands_LDADD = libebus-$(LIBEBUS_API_VERSION).la

test_config_SOURCES = test/test_config.cpp
test_config_LDADD = libebus-$(LIBEBUS_API_VERSION).la

test_decode_SOURCES = test/test_decode.cpp
test_decode_LDADD = libebus-$(LIBEBUS_API_VERSION).la

test_encode_SOURCES = test/test_encode.cpp
test_encode_LDADD = libebus-$(LIBEBUS_API_VERSION).la
