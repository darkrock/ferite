## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = number*.h number*.c number*~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite
endif

LDFLAGS          = $(libferite) -L${libdir} @number_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @number_CFLAGS@
DEFS             = @thread_defs@

scripts_DATA     = number.fec 
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@

modxml_DATA      = number.xml
modxmldir        = @FE_LIBRARY_PATH@/module-descriptions

EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA)
pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = number.la

number_la_SOURCES    = number_core.c number_misc.c number_Number.c number_header.h  
number_la_LDFLAGS    = -no-undefined -module -avoid-version
number_la_LIBADD     =

number_core.c: @MODULE_SRC_PREFIX@/number/number.fec 
	@BUILDER@ -m number @MODULE_SRC_PREFIX@/number/number.fec

number_misc.c: number_core.c
number_Number.c: number_core.c
number_header.h: number_core.c
