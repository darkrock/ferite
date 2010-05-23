
## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS     = 1.4 foreign

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = INSTALL Makefile.in aclocal.m4 config.guess \
		       config.h.in config.sub configure install-sh \
		       ltconfig ltmain.sh missing mkinstalldirs \
		       stamp-h.in

SUBDIRS = libs src builder test include modules scripts tools docs

bin_SCRIPTS = ferite-config

fbm_DATA = LICENSE AUTHORS TODO ROADMAP DEVELOPERS ABOUT RELEASE.NOTES
fbmdir = $(prefix)/share/doc/ferite

EXTRA_DIST = shtool $(fbm_DATA) $(m4data_DATA) ferite.spec modules-config.m4

m4datadir   = $(datadir)/aclocal
m4data_DATA = ferite.m4
