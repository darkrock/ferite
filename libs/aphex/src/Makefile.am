## Process this file with automake to produce Makefile.in -*- makefile -*-

AUTOMAKE_OPTIONS     = no-dependencies foreign

# A list of all the files in the current directory which can be regenerated

MAINTAINERCLEANFILES = Makefile.in

LDFLAGS		         = -L${prefix}/lib

INCLUDES	         = -I${top_srcdir}/libs/aphex/include -I${top_srcdir}/include -I${prefix}/include -I$(includedir) @aphex_INCS@
                       
			
DEFS                 = @debugdefs@ @thread_defs@ @gc_defs@

noinst_LTLIBRARIES   = libaphex.la

include_HEADERS      =

libaphex_la_DEPENDENCIES = $(top_builddir)/config.h
libaphex_la_LDFLAGS      = -no-undefined $(LDFLAGS) -m @aphex_LDFLAGS@
libaphex_la_LIBADD       = @thread_libs@ @gc_libs@

libaphex_la_SOURCES  = aphex_file.c aphex_directory.c aphex_config.c aphex_match.c aphex_thread.c aphex_notify.c poll.c poll.h
