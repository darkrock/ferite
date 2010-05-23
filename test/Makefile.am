bin_PROGRAMS = ferite cache_ferite

ferite_SOURCES = main.c dot.c dot.h
cache_ferite_SOURCES = cache-ferite.c

EXTRA_DIST = dot.h
		    
ferite_DEPENDENCIES = $(top_builddir)/src/libferite.la
ferite_LDFLAGS =

cache_ferite_DEPENDENCIES = $(top_builddir)/src/libferite.la
cache_ferite_LDFLAGS =

INCLUDES = -I. -I$(top_srcdir) -I$(top_srcdir)/src -I..     \
-I$(top_srcdir)/libs/aphex/include -I$(top_srcdir)/include  \
-I$(includedir) -I$(prefix)/include

DEFS=@thread_defs@  @gc_defs@

LDADD = $(top_builddir)/src/libferite.la -L$(libdir) -L$(prefix)/lib @gc_libs@ 
