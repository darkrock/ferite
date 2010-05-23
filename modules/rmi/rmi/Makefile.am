
## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = 

SUBDIRS		     = transport translation protocol

scripts_DATA = RMIProtocolLayer.fe RMIRemoteObject.fe RMITranslationLayer.fe RMITransportLayer.fe RMILocalVariable.fe
scriptsdir = @FE_XPLAT_LIBRARY_PATH@/rmi

EXTRA_DIST = $(scripts_DATA)
