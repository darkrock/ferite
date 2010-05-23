## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = Makefile.in

LDFLAGS          = -L${libdir} @moduledefs@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I.

pkgdir           = $(libdir)/ferite/modules
pkg_LTLIBRARIES  = example_module.la

example_module_la_SOURCES    = example_module_core.c example_module_foo.c 
example_module_la_LDFLAGS    = -no-undefined -module -avoid-version
example_module_la_LIBADD     =
