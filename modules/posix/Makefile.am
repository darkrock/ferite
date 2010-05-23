## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = posix*.h posix*.c posix*~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite -L${top_builddir}/modules/stream -lferitestream
endif

LDFLAGS          = $(libferite) -L${libdir} @posix_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @posix_CFLAGS@
DEFS             = @thread_defs@

scripts_DATA   = posix.fec posix_consts.fec
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@


modxml_DATA      = posix.xml
modxmldir        = @FE_LIBRARY_PATH@/module-descriptions
EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA) consts.txt consts.h consts.sh
pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = posix.la

posix_la_SOURCES    = posix_core.c posix_misc.c posix_Posix.c posix_Posix_Uname.c posix_Posix_Rlimit.c posix_Posix_User.c posix_Posix_Pipe.c posix_Posix_Rusage.c posix_Posix_ProcessStream.c posix_Posix_Stat.c posix_Posix_SelectResult.c posix_Posix_Group.c posix_header.h  util_posix.c util_posix.h util_user.c util_user.h util_signal.c util_signal.h
posix_la_LDFLAGS    = -no-undefined -module -avoid-version
posix_la_LIBADD     =

posix_core.c: @MODULE_SRC_PREFIX@/posix/posix.fec @MODULE_SRC_PREFIX@/posix/posix_consts.fec
	@BUILDER@ -m posix @MODULE_SRC_PREFIX@/posix/posix.fec

consts.h: consts.txt consts.sh
	@MODULE_SRC_PREFIX@/posix/consts.sh @MODULE_SRC_PREFIX@/posix/consts.txt > @MODULE_SRC_PREFIX@/posix/consts.h

posix_misc.c : posix_core.c
posix_Posix.c : posix_core.c
posix_Posix_Uname.c : posix_core.c
posix_Posix_Rlimit.c : posix_core.c
posix_Posix_User.c : posix_core.c
posix_Posix_Pipe.c : posix_core.c
posix_Posix_Pipe.c : posix_core.c
posix_Posix_Rusage.c : posix_core.c
posix_Posix_ProcessStream.c : posix_core.c
posix_Posix_Stat.c : posix_core.c
posix_Posix_SelectResult.c : posix_core.c
posix_Posix_Group.c : posix_core.c
posix_header.h : posix_core.c
