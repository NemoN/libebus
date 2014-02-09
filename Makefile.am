AUTOMAKE_OPTIONS = subdir-objects

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

AM_CPPFLAGS = -Wall -Wextra

lib_LTLIBRARIES = libebus-@LIBEBUS_API_VERSION@.la

libebus_@LIBEBUS_API_VERSION@_la_SOURCES = libebus/Port.cpp \
					   libebus/Bus.cpp \
					   libebus/Command.cpp \
					   libebus/Config.cpp

libebus_@LIBEBUS_API_VERSION@_la_LDFLAGS = -version-info $(LIBEBUS_SO_VERSION)

libebus_includedir = $(includedir)/libebus-$(LIBEBUS_API_VERSION)
nobase_libebus_include_HEADERS = libebus.hpp \
                                 libebus/Port.hpp \
                                 libebus/Bus.hpp \
                                 libebus/Command.hpp \
                                 libebus/Config.hpp

libebus_libincludedir = $(libdir)/libebus-$(LIBEBUS_API_VERSION)/include
nodist_libebus_libinclude_HEADERS = libebusconfig.hpp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libebus-$(LIBEBUS_API_VERSION).pc

dist_noinst_SCRIPTS = autogen.sh


noinst_PROGRAMS = test_port \
		  test_bus \
		  test_command \
		  test_config

test_port_SOURCES = test/test_port.cpp
test_port_LDADD = libebus-$(LIBEBUS_API_VERSION).la

test_bus_SOURCES = test/test_bus.cpp
test_bus_LDADD = libebus-$(LIBEBUS_API_VERSION).la

test_command_SOURCES = test/test_command.cpp
test_command_LDADD = libebus-$(LIBEBUS_API_VERSION).la

test_config_SOURCES = test/test_config.cpp
test_config_LDADD = libebus-$(LIBEBUS_API_VERSION).la
