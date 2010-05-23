## Process this file with automake to produce Makefile.in -*- makefile -*-

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated

MAINTAINERCLEANFILES = Makefile.in

CLEANFILES       = *~

LDFLAGS		     = -L${prefix}/lib -lm

INCLUDES	     = -I$(top_srcdir) -I$(top_srcdir)/libs/aphex/include -I$(top_srcdir)/libs/triton/include -I${top_srcdir}/include -I${prefix}/include -I$(includedir) -I${top_srcdir}/include/ferite/ @pcre_cflags@

DEFS                 = @debugdefs@ @thread_defs@ @gc_defs@

lib_LTLIBRARIES      = libferite.la

include_HEADERS      =

libferite_la_SOURCES = \
ferite_gc_generation.c \
     ferite_gc_libgc.c \
  ferite_mem_classic.c \
     ferite_mem_jedi.c \
    ferite_mem_libgc.c \
    ferite_variables.c \
    ferite_namespace.c \
     ferite_function.c \
      ferite_compile.c \
      ferite_execute.c \
      ferite_scanner.c \
      ferite_globals.c \
       ferite_script.c \
       ferite_parser.c \
       ferite_opcode.c \
       ferite_module.c \
       ferite_string.c \
       ferite_buffer.c \
       ferite_uarray.c \
       ferite_thread.c \
        ferite_error.c \
        ferite_utils.c \
        ferite_stack.c \
        ferite_regex.c \
        ferite_class.c \
         ferite_hash.c \
          ferite_obj.c \
          ferite_ops.c \
          ferite_amt.c \
     ferite_amtarray.c \
        ferite_cache.c \
           ferite_gc.c \
              ferite.c

compiler: ferite_parser.c ferite_scanner.c
	rm -f ferite_parser.output
	bison -v -d -p fep -b ferite_parser ferite_parser.y
	mv ferite_parser.tab.c ferite_parser.c
	mv ferite_parser.tab.h ../include/ferite/fparser.h
	flex -b -Cfr -Pfep -8 ferite_scanner.l
	mv lex.fep.c ferite_scanner.c
	rm lex.backup

ferite_parser.c: ferite_parser.y
ferite_scanner.c: ferite_scanner.l

EXTRA_DIST = ferite_parser.y ferite_scanner.l

libferite_la_LIBADD       = $(top_builddir)/libs/triton/src/libtriton.la \
                            $(top_builddir)/libs/aphex/src/libaphex.la \
							-lm @pcre_libs@ @thread_libs@ @gc_libs@ $(LDFLAGS)	 
libferite_la_DEPENDENCIES = $(top_builddir)/config.h $(top_builddir)/libs/triton/src/libtriton.la $(top_builddir)/libs/aphex/src/libaphex.la
libferite_la_LDFLAGS      = -version-info 1:1:0 -export-dynamic -no-undefined
