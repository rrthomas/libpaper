man_MANS = \
	paperconf.1 \
	libpaper.3 \
	papersize.5 \
	paperconfig.8

EXTRA_DIST = \
	$(man_MANS) \
	paperconf.1.in.in \
	libpaper.3.in.in \
	papersize.5.in.in \
	paperconfig.8.in.in

DISTCLEANFILES = $(generated_MANS)

edit = sed -e 's|$$[{]prefix[}]|$(prefix)|g'

paperconf.1: $(builddir)/paperconf.1.in
	$(edit) $(builddir)/$@.in >$@

libpaper.3: $(builddir)/libpaper.3.in
	$(edit) $(builddir)/$@.in >$@

papersize.5: $(builddir)/papersize.5.in
	rm -f $@ $@.tmp
	paperspecslist="`cut -d ' ' -f 1 < $(top_srcdir)/libpaper/paperspecs | tr '\n' ' ' | sed -e 's/ /, /g;s/, $$//'`" && \
		sed -e "s|@paperspecslist[@]|$$paperspecslist|g" $(builddir)/$@.in >$@.tmp
	mv $@.tmp $@

paperconfig.8: $(builddir)/paperconfig.8.in
	$(edit) $(builddir)/$@.in >$@
