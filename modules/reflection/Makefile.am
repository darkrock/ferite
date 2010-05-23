## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = reflection*.h reflection*.c reflection*~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite
endif

LDFLAGS          = $(libferite) -L${libdir} @reflection_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @reflection_CFLAGS@
DEFS             = @thread_defs@

scripts_DATA     = reflection.fec 
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@

modxml_DATA      = reflection.xml
modxmldir        = @FE_LIBRARY_PATH@/module-descriptions

EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA)
pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = reflection.la

reflection_la_SOURCES    = reflection_core.c reflection_misc.c reflection_Reflection.c reflection_Namespace.c reflection_Function.c reflection_Class.c reflection_Variable.c reflection_Object.c reflection_header.h  utility.c 
reflection_la_LDFLAGS    = -no-undefined -module -avoid-version
reflection_la_LIBADD     =

reflection_core.c: @MODULE_SRC_PREFIX@/reflection/reflection.fec 
	@BUILDER@ -m reflection @MODULE_SRC_PREFIX@/reflection/reflection.fec

reflection_misc.c: reflection_core.c
reflection_Reflection.c: reflection_core.c
reflection_Function.c: reflection_core.c
reflection_Namespace.c: reflection_core.c
reflection_Class.c: reflection_core.c
reflection_Variable.c: reflection_core.c
reflection_Object.c: reflection_core.c
reflection_header.h: reflection_core.c
