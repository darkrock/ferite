#Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = xml*.h xml*.c xml*~~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite
endif

LDFLAGS          = $(libferite) -L${libdir} @xml_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @xml_CFLAGS@
DEFS             = @thread_defs@

scripts_DATA     = xml.fec
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@

modxml_DATA      = xml.xml
modxmldir        = @FE_LIBRARY_PATH@/module-descriptions

EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA)
pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = xml.la

xml_la_SOURCES    = xml_core.c xml_misc.c xml_XML.c xml_XML_SAXParser.c xml_XML_TreeParser.c xml_header.h  sax_handlers.c sax_handlers.h tree_handlers.c tree_handlers.h xpath_handlers.c xpath_handlers.h xml_XML_Element.c
xml_la_LDFLAGS    = -no-undefined -module -avoid-version
xml_la_LIBADD     =

xml_core.c: @MODULE_SRC_PREFIX@/xml/xml.fec 
	@BUILDER@ -m xml @MODULE_SRC_PREFIX@/xml/xml.fec

xml_misc.c xml_SAXParser.c xml_treeparser.c xml_header.h : xml_core.c
xml_SAXParser.c xml_treeparser.c xml_header.h : xml_core.c
xml_treeparser.c xml_header.h : xml_core.c
xml_header.h : xml_core.c

