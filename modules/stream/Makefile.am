## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = stream*.h stream*.c stream*~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite
endif

LDFLAGS          = $(libferite) -L${libdir} @stream_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @stream_CFLAGS@
DEFS             = @thread_defs@

lib_LTLIBRARIES  = libferitestream.la
libferitestream_la_SOURCES = util_stream.c
libferitestream_la_LIBADD       = $(LDFLAGS)	 
libferitestream_la_DEPENDENCIES = 
libferitestream_la_LDFLAGS      = -version-info 1:0:0 -export-dynamic -no-undefined

fsl_DATA         = util_stream.h
fsldir           = $(prefix)/include/ferite/stream

scripts_DATA     = stream.fec
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@

modxml_DATA      = stream.xml
modxmldir        = @FE_LIBRARY_PATH@/module-descriptions
EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA) $(fsl_DATA)

pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = stream.la

stream_la_SOURCES    = stream_core.c stream_misc.c stream_Stream.c stream_Stream_Stream.c stream_Stream_StdioStream.c stream_Stream_StringStream.c stream_Stream_PointerObject.c stream_header.h  util_stream.c util_stream.h 
stream_la_LDFLAGS    = -no-undefined -module -avoid-version  -L. -lferitestream
stream_la_LIBADD     =

stream_core.c: @MODULE_SRC_PREFIX@/stream/stream.fec
	@BUILDER@ -m stream @MODULE_SRC_PREFIX@/stream/stream.fec

stream_misc.c stream_Stream.c stream_Stream_Stream.c stream_Stream_StdioStream.c stream_Stream_StringStream.c stream_Stream_PointerObject.c stream_header.h : stream_core.c
stream_Stream.c stream_Stream_Stream.c stream_Stream_StdioStream.c stream_Stream_StringStream.c stream_Stream_PointerObject.c stream_header.h : stream_core.c
stream_Stream_Stream.c stream_Stream_StdioStream.c stream_Stream_StringStream.c stream_Stream_PointerObject.c stream_header.h : stream_core.c
stream_Stream_StdioStream.c stream_Stream_StringStream.c stream_Stream_PointerObject.c stream_header.h : stream_core.c
stream_Stream_StringStream.c stream_Stream_PointerObject.c stream_header.h : stream_core.c
stream_Stream_PointerObject.c stream_header.h : stream_core.c
stream_header.h : stream_core.c
