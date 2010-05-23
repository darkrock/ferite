
EXTRA_DIST = makeconfig.fe

install-data-local:
	$(prefix)/bin/ferite $(top_srcdir)/scripts/config/makeconfig.fe; \
	cp $(top_builddir)/scripts/config/config.feh @FE_XPLAT_LIBRARY_PATH@;
