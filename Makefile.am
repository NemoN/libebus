AUTOMAKE_OPTIONS = subdir-objects

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

AM_CPPFLAGS = -Wall -Wextra

lib_LTLIBRARIES = libebus-@LIBEBUS_API_VERSION@.la

libebus_@LIBEBUS_API_VERSION@_la_SOURCES = libebus/port.cpp \
					   libebus/bus.cpp \
					   libebus/commands.cpp \
					   libebus/config.cpp \
					   libebus/dump.cpp

libebus_@LIBEBUS_API_VERSION@_la_LDFLAGS = -version-info $(LIBEBUS_SO_VERSION)

libebus_includedir = $(includedir)/libebus-$(LIBEBUS_API_VERSION)
nobase_libebus_include_HEADERS = libebus.hpp \
                                 libebus/port.hpp \
                                 libebus/bus.hpp \
                                 libebus/commands.hpp \
                                 libebus/config.hpp \
                                 libebus/dump.hpp

libebus_libincludedir = $(libdir)/libebus-$(LIBEBUS_API_VERSION)/include
nodist_libebus_libinclude_HEADERS = libebusconfig.hpp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libebus-$(LIBEBUS_API_VERSION).pc

dist_noinst_SCRIPTS = autogen.sh


noinst_PROGRAMS = test_port \
		  test_bus \
		  test_commands \
		  test_config

test_port_SOURCES = test/test_port.cpp
test_port_LDADD = libebus-$(LIBEBUS_API_VERSION).la

test_bus_SOURCES = test/test_bus.cpp
test_bus_LDADD = libebus-$(LIBEBUS_API_VERSION).la

test_commands_SOURCES = test/test_commands.cpp
test_commands_LDADD = libebus-$(LIBEBUS_API_VERSION).la

test_config_SOURCES = test/test_config.cpp
test_config_LDADD = libebus-$(LIBEBUS_API_VERSION).la
