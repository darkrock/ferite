  
gen_DATA = generate-module.fe
gendir = @prefix@/share/ferite/generate-module

bin_SCRIPTS = generate-module

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/skel; then \
	  $(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/generate-module; \
	  (cd $(srcdir); tar -cf - skel) | (cd $(DESTDIR)$(pkgdatadir)/generate-module; tar -xf -) \
	fi

uninstall-local:
	@$(NORMAL_UNINSTALL)
	rm -rf $(DESTDIR)$(pkgdatadir)/generate-module

EXTRA_DIST = $(gen_DATA) skel/AUTHORS skel/autogen.sh skel/ChangeLog skel/config.h.in \
        skel/configure.ac skel/Makefile.am skel/README \
        skel/stamp-h.in skel/udcl.sh
