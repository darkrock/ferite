## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = test*.h test*.c test*~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite
endif

LDFLAGS          = $(libferite) -L${libdir} @test_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @test_CFLAGS@
DEFS             = @thread_defs@

scripts_DATA     = test.fec 
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@

modxml_DATA      = test.xml
modxmldir        = @FE_LIBRARY_PATH@/module-descriptions

EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA)
pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = test.la

test_la_SOURCES    = test_core.c test_misc.c test_Test.c test_header.h  utility.c utility.h 
test_la_LDFLAGS    = -no-undefined -module -avoid-version
test_la_LIBADD     =

test_core.c: @MODULE_SRC_PREFIX@/test/test.fec 
	@BUILDER@ -m test @MODULE_SRC_PREFIX@/test/test.fec

test_misc.c: test_core.c
test_Test.c: test_core.c
test_header.h: test_core.c
