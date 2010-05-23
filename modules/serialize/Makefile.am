## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = serialize*.h serialize*.c serialize*~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite
endif

LDFLAGS          = $(libferite) -L${libdir} @serialize_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @serialize_CFLAGS@
DEFS             = @thread_defs@

scripts_DATA     = serialize.fec serialize-xml.feh
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@

modxml_DATA      = serialize.xml
modxmldir        = @FE_LIBRARY_PATH@/module-descriptions

EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA)
pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = serialize.la

serialize_la_SOURCES    = serialize_core.c serialize_misc.c serialize_Serialize.c serialize_header.h  utility.c utility.h 
serialize_la_LDFLAGS    = -no-undefined -module -avoid-version
serialize_la_LIBADD     =

serialize_core.c: @MODULE_SRC_PREFIX@/serialize/serialize.fec 
	@BUILDER@ -m serialize @MODULE_SRC_PREFIX@/serialize/serialize.fec

serialize_misc.c: serialize_core.c
serialize_Serialize.c: serialize_core.c
serialize_header.h: serialize_core.c
