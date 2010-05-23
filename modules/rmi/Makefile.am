
## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = 

SUBDIRS		     = rmi

scripts_DATA = rmi.fe
scriptsdir = @FE_XPLAT_LIBRARY_PATH@

modxml_DATA      = rmi.xml
modxmldir        = @FE_LIBRARY_PATH@/module-descriptions

EXTRA_DIST = $(scripts_DATA) $(modxml_DATA)
