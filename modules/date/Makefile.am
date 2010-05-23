## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = date*.h date*.c date*~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite -L${top_builddir}/modules/stream -lferitestream
endif

LDFLAGS          = $(libferite) -L${libdir} @date_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @date_CFLAGS@
DEFS             = @thread_defs@

scripts_DATA     = date.fec
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@


modxml_DATA      = date.xml
modxmldir        = @FE_LIBRARY_PATH@/module-descriptions
EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA)
pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = date.la

date_la_SOURCES    = date_core.c date_header.h date_misc.c date_Alarm.c date_Date.c util_date.c util_date.h strptime.c
date_la_LDFLAGS    = -no-undefined -module -avoid-version
date_la_LIBADD     =

date_core.c: @MODULE_SRC_PREFIX@/date/date.fec
	@BUILDER@ -m date @MODULE_SRC_PREFIX@/date/date.fec

date_misc.c : date_core.c
date_Date.c : date_core.c
date_Alarm.c : date_core.c
date_header.h : date_core.c
