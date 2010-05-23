bin_PROGRAMS = builder

builder_SOURCES = builder_main.c
		    
builder_DEPENDENCIES = $(top_builddir)/src/libferite.la
builder_LDFLAGS = 

INCLUDES = -I. -I$(top_srcdir) -I$(top_srcdir)/src -I..     \
-I$(top_srcdir)/libs/aphex/include -I$(top_srcdir)/include  \
-I$(includedir) -I$(prefix)/include

DEFS=@thread_defs@  @gc_defs@

LDADD = $(top_builddir)/src/libferite.la -L$(libdir) -L$(prefix)/lib @gc_libs@

EXTRA_DIST = builder.h
