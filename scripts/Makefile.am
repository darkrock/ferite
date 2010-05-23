
## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = 

SUBDIRS		     = test cgi test-suite xmlrpc

modxml_DATA  = misc.xml
modxmldir    = @FE_LIBRARY_PATH@/module-descriptions

scripts_DATA = GetOptLong.fe freezethaw.fe twitter.feh
scriptsdir   = @FE_XPLAT_LIBRARY_PATH@

EXTRA_DIST   = $(scripts_DATA) $(modxml_DATA)
