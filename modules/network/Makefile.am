## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = network*.h network*.c network*~

CLEANFILES       = 

if NEED_FERITE_LIB
libferite = -L${top_builddir}/src -lferite -L${top_builddir}/modules/stream -lferitestream
endif

LDFLAGS          = $(libferite) -L${libdir} @network_LIBS@
INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @network_CFLAGS@
DEFS             = @thread_defs@

scripts_DATA   = network.fec
scriptsdir       = @FE_XPLAT_LIBRARY_PATH@


modxml_DATA    = network.xml
modxmldir       = @FE_LIBRARY_PATH@/module-descriptions
EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA)
pkgdir           = @FE_NATIVE_LIBRARY_PATH@
pkg_LTLIBRARIES  = network.la

network_la_SOURCES    = network_core.c network_misc.c network_Network.c network_Network_TCP.c network_Network_TCP_Stream.c  network_Network_Host.c network_Network_UDP.c network_Network_UDP_Stream.c network_Network_Service.c network_header.h  util_network.c util_network.h 
network_la_LDFLAGS    = -no-undefined -module -avoid-version
network_la_LIBADD     =

network_core.c: @MODULE_SRC_PREFIX@/network/network.fec 
	@BUILDER@ -m network @MODULE_SRC_PREFIX@/network/network.fec

network_misc.c network_Network.c  : network_core.c
network_Network_TCP_Stream.c  network_Network.c : network_core.c
network_Network.c : network_core.c
network_Network_Host.c: network_core.c
network_Network_UDPSocket.c: network_core.c
network_Network_Service.c : network_core.c
network_header.h : network_core.c
