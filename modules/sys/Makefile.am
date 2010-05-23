## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = sys*.h sys*.c sys*~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite -L${top_builddir}/modules/stream -lferitestream
endif

LDFLAGS          = $(libferite) -L${libdir} @sys_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @sys_CFLAGS@
DEFS             = @thread_defs@

scripts_DATA     = env.fec std.fec sys.fec 
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@

modxml_DATA      = sys.xml
modxmldir        = @FE_LIBRARY_PATH@/module-descriptions

EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA)
pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = sys.la

sys_la_SOURCES    = sys_core.c sys_misc.c sys_Sys.c sys_Sys_env.c sys_header.h   
sys_la_LDFLAGS    = -no-undefined -module -avoid-version
sys_la_LIBADD     =

sys_core.c: @MODULE_SRC_PREFIX@/sys/env.fec @MODULE_SRC_PREFIX@/sys/std.fec @MODULE_SRC_PREFIX@/sys/sys.fec
	@BUILDER@ -m sys @MODULE_SRC_PREFIX@/sys/sys.fec

sys_misc.c: sys_core.c
sys_Sys.c: sys_core.c
sys_Sys_env.c: sys_core.c
sys_header.h: sys_core.c
