## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = unix*.h unix*.c unix*~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite -L${top_builddir}/modules/stream -lferitestream
endif

LDFLAGS          = $(libferite) -L${libdir} @unix_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @unix_CFLAGS@
DEFS             = @thread_defs@

scripts_DATA   = unix.fec syslog.fec unixsocket.fec
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@


modxml_DATA    = unix.xml
modxmldir       = @FE_LIBRARY_PATH@/module-descriptions
EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA) consts.h consts.sh consts.txt
pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = unix.la

unix_la_SOURCES    = unix_core.c unix_misc.c unix_Unix.c unix_Unix_SyslogStream.c unix_Network.c unix_Network_Unix.c unix_Network_Unix_Stream.c unix_header.h  
unix_la_LDFLAGS    = -no-undefined -module -avoid-version
unix_la_LIBADD     =

unix_core.c: @MODULE_SRC_PREFIX@/unix/unix.fec @MODULE_SRC_PREFIX@/unix/syslog.fec @MODULE_SRC_PREFIX@/unix/unixsocket.fec
	@BUILDER@ -m unix @MODULE_SRC_PREFIX@/unix/unix.fec

consts.h: consts.txt consts.sh
	@MODULE_SRC_PREFIX@/unix/consts.sh @MODULE_SRC_PREFIX@/unix/consts.txt > @MODULE_SRC_PREFIX@/unix/consts.h

unix_misc.c: unix_core.c
unix_Unix.c : unix_core.c
unix_Unix_SyslogStream.c : unix_core.c
unix_header.h : unix_core.c
