## Process this file with automake to produce Makefile.in -*- makefile -*-

AUTOMAKE_OPTIONS     = no-dependencies foreign

# A list of all the files in the current directory which can be regenerated

MAINTAINERCLEANFILES = Makefile.in

LDFLAGS		         = -L/usr/local/lib

INCLUDES	         = -I${top_srcdir} -I${top_srcdir}/libs/triton/include -I${top_srcdir}/include -I${prefix}/include -I$(includedir) @TRITON_INCS@

DEFS                 = @debugdefs@

## if INSTALL_TRITON
## lib_LTLIBRARIES   = libtriton.la
## else
noinst_LTLIBRARIES   = libtriton.la
## endif

CLEANFILES = libltdl.la libltdlc.la

include_HEADERS      =

libtriton_la_DEPENDENCIES = $(top_builddir)/config.h
libtriton_la_LDFLAGS      = -no-undefined $(LDFLAGS) -m @TRITON_LDFLAGS@

libtriton_la_SOURCES = triton_core.c  triton_loader_dlfcn.c triton_loader_ltdl.c triton_loader_darwin.c triton_loader_win32.c
