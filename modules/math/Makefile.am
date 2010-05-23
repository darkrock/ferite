## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = math*.h math*.c math*~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite
endif

LDFLAGS          = $(libferite) -L${libdir} @math_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @math_CFLAGS@
DEFS             = @thread_defs@

scripts_DATA     = complex.fec math.fec 
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@

modxml_DATA      = math.xml
modxmldir        = @FE_LIBRARY_PATH@/module-descriptions

EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA)
pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = math.la

math_la_SOURCES    = math_core.c math_misc.c math_Math.c math_Math_Complex.c math_header.h  
math_la_LDFLAGS    = -no-undefined -module -avoid-version
math_la_LIBADD     =

math_core.c: @MODULE_SRC_PREFIX@/math/complex.fec @MODULE_SRC_PREFIX@/math/math.fec 
	@BUILDER@ -m math @MODULE_SRC_PREFIX@/math/math.fec

math_misc.c: math_core.c
math_Math.c: math_core.c
math_Math_Complex.c: math_core.c
math_header.h: math_core.c
