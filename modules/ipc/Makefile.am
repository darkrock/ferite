## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = ipc*.h ipc*.c ipc*~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite
endif

LDFLAGS          = $(libferite) -L${libdir} @ipc_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @ipc_CFLAGS@
DEFS             = @thread_defs@

scripts_DATA     = ipc.fec 
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@

modxml_DATA      = ipc.xml
modxmldir        = @FE_LIBRARY_PATH@/module-descriptions

EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA)
pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = ipc.la

ipc_la_SOURCES    = ipc_core.c ipc_misc.c ipc_IPCObject.c ipc_header.h  
ipc_la_LDFLAGS    = -no-undefined -module -avoid-version
ipc_la_LIBADD     =

ipc_core.c: @MODULE_SRC_PREFIX@/ipc/ipc.fec 
	@BUILDER@ -m ipc @MODULE_SRC_PREFIX@/ipc/ipc.fec

ipc_misc.c: ipc_core.c
ipc_IpcObject.c: ipc_core.c
ipc_header.h: ipc_core.c
