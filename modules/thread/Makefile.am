## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = thread*.h thread*.c thread*~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite
endif

LDFLAGS          = $(libferite) -L${libdir} @thread_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @thread_CFLAGS@
DEFS             = @thread_defs@

scripts_DATA     = thread.fec 
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@

modxml_DATA      = thread.xml
modxmldir        = @FE_LIBRARY_PATH@/module-descriptions

EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA)
pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = thread.la

thread_la_SOURCES    = thread_core.c thread_misc.c thread_Thread.c thread_Mutex.c thread_Event.c thread_header.h  utility.c 
thread_la_LDFLAGS    = -no-undefined -module -avoid-version
thread_la_LIBADD     =

thread_core.c: @MODULE_SRC_PREFIX@/thread/thread.fec 
	@BUILDER@ -m thread @MODULE_SRC_PREFIX@/thread/thread.fec

thread_misc.c: thread_core.c
thread_Thread.c: thread_core.c
thread_Mutex.c: thread_core.c
thread_Event.c: thread_core.c
thread_header.h: thread_core.c
