## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = string*.h string*.c string*~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite
endif

LDFLAGS          = $(libferite) -L${libdir} @string_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @string_CFLAGS@
DEFS             = @thread_defs@

scripts_DATA     = string.fec 
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@

modxml_DATA      = string.xml
modxmldir        = @FE_LIBRARY_PATH@/module-descriptions

EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA)
pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = string.la

string_la_SOURCES    = string_core.c string_misc.c string_String.c string_header.h  
string_la_LDFLAGS    = -no-undefined -module -avoid-version
string_la_LIBADD     =

string_core.c: @MODULE_SRC_PREFIX@/string/string.fec 
	@BUILDER@ -m string @MODULE_SRC_PREFIX@/string/string.fec

string_misc.c: string_core.c
string_String.c: string_core.c
string_header.h: string_core.c
