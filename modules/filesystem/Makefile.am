## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = filesystem*.h filesystem*.c filesystem*~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite  -L${top_builddir}/modules/stream -lferitestream
endif

LDFLAGS          = $(libferite) -L${libdir} @filesystem_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @filesystem_CFLAGS@
DEFS             = @thread_defs@

scripts_DATA   = filesystem.fec
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@


modxml_DATA    = filesystem.xml
modxmldir       = @FE_LIBRARY_PATH@/module-descriptions
EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA) consts.txt consts.h consts.sh
pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = filesystem.la

filesystem_la_SOURCES    = filesystem_core.c filesystem_misc.c filesystem_FileSystem.c filesystem_Directory.c filesystem_File.c filesystem_header.h  
filesystem_la_LDFLAGS    = -no-undefined -module -avoid-version
filesystem_la_LIBADD     =

filesystem_core.c: @MODULE_SRC_PREFIX@/filesystem/filesystem.fec
	@BUILDER@ -m filesystem @MODULE_SRC_PREFIX@/filesystem/filesystem.fec

consts.h: consts.txt consts.sh
	@MODULE_SRC_PREFIX@/filesystem/consts.sh @MODULE_SRC_PREFIX@/filesystem/consts.txt > @MODULE_SRC_PREFIX@/filesystem/consts.h

filesystem_misc.c filesystem_FileSystem.c filesystem_Directory.c filesystem_File.c filesystem_header.h : filesystem_core.c
filesystem_FileSystem.c filesystem_Directory.c filesystem_File.c filesystem_header.h : filesystem_core.c
filesystem_Directory.c filesystem_File.c filesystem_header.h : filesystem_core.c
filesystem_File.c filesystem_header.h : filesystem_core.c
filesystem_header.h : filesystem_core.c
