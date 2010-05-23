
docs_DATA = manual.pdf embed.pdf feritedoc.pdf

man_MANS = ferite.1 builder.1

manual = html-manuals/manual/index.html html-manuals/manual/style.css \
	html-manuals/manual/http.png html-manuals/manual/http-int.png \
	html-manuals/manual/docs-background.png
	
embed = html-manuals/embed/index.html html-manuals/embed/style.css \
	html-manuals/embed/http.png html-manuals/embed/http-int.png \
	html-manuals/embed/docs-background.png
	
feritedoc = html-manuals/feritedoc/index.html html-manuals/feritedoc/style.css \
	html-manuals/feritedoc/http.png html-manuals/feritedoc/http-int.png \
	html-manuals/feritedoc/docs-background.png

docsdir = $(prefix)/share/doc/ferite

EXTRA_DIST = $(docs_DATA) $(man_MANS) build_c_api_docs.sh $(feritedoc) $(manual) $(embed)

SUBDIRS = images

capi:
	if test -z "$$DESTDIR"; then \
		rm -rf generated/capi && $(srcdir)/build_c_api_docs.sh $(prefix)/bin/; \
	fi

install-data-local: capi
	@$(NORMAL_INSTALL)
	if test -z "$$DESTDIR"; then \
		$(mkinstalldirs) $(prefix)/share/doc/ferite/api; \
		$(prefix)/bin/feritedoc --regenerate; \
		if test -d $(srcdir)/generated/capi; then \
		  $(mkinstalldirs) $(prefix)/share/doc/ferite/capi; \
		  (cd $(srcdir)/generated; tar -cf - capi) | (cd $(prefix)/share/doc/ferite/; tar -xf -) \
		fi; \
	fi; \
	(cd $(srcdir); tar -cf - html-manuals) | (cd $$DESTDIR$(prefix)/share/doc/ferite/; tar -xf -)
