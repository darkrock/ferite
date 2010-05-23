## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = array*.h array*.c array*~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite
endif

LDFLAGS          = $(libferite) -L${libdir} @array_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @array_CFLAGS@
DEFS             = @thread_defs@

scripts_DATA     = array.fec 
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@

modxml_DATA      = array.xml
modxmldir        = @FE_LIBRARY_PATH@/module-descriptions

EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA)
pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = array.la

array_la_SOURCES    = array_core.c array_misc.c array_Array.c array_header.h  util.c util.h 
array_la_LDFLAGS    = -no-undefined -module -avoid-version
array_la_LIBADD     =

array_core.c: @MODULE_SRC_PREFIX@/array/array.fec 
	@BUILDER@ -m array @MODULE_SRC_PREFIX@/array/array.fec

array_misc.c: array_core.c
array_Array.c: array_core.c
array_header.h: array_core.c
