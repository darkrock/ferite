## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = regexp*.h regexp*.c regexp*~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite
endif

LDFLAGS          = $(libferite) -L${libdir} @regexp_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @regexp_CFLAGS@
DEFS             = @thread_defs@

scripts_DATA     = regexp.fec 
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@

modxml_DATA      = regexp.xml
modxmldir        = @FE_LIBRARY_PATH@/module-descriptions

EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA)
pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = regexp.la

regexp_la_SOURCES    = regexp_core.c regexp_misc.c regexp_MatchObject.c regexp_Regexp.c regexp_header.h  
regexp_la_LDFLAGS    = -no-undefined -module -avoid-version
regexp_la_LIBADD     =

regexp_core.c: @MODULE_SRC_PREFIX@/regexp/regexp.fec  
	@BUILDER@ -m regexp @MODULE_SRC_PREFIX@/regexp/regexp.fec

regexp_misc.c regexp_MatchObject.c regexp_Regexp.c regexp_header.h : regexp_core.c
regexp_MatchObject.c regexp_Regexp.c regexp_header.h : regexp_core.c
regexp_Regexp.c regexp_header.h : regexp_core.c
regexp_header.h : regexp_core.c
