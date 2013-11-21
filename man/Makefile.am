man_MANS = \
	paperconfig.1 \
	paperinfo.1 \
	libpaper.3 \
	paperspecs.5

EXTRA_DIST = \
	$(man_MANS) \
	paperconfig.1.in.in \
	paperinfo.1.in.in \
	libpaper.3.in.in \
	paperspecs.5.in.in

DISTCLEANFILES = $(generated_MANS)

edit = sed -e 's|$$[{]prefix[}]|$(prefix)|g'

paperconfig.1: $(builddir)/paperconfig.1.in
	$(edit) $(builddir)/$@.in >$@

paperinfo.1: $(builddir)/paperinfo.1.in
	$(edit) $(builddir)/$@.in >$@

libpaper.3: $(builddir)/libpaper.3.in
	$(edit) $(builddir)/$@.in >$@

paperspecs.5: $(builddir)/paperspecs.5.in
	$(edit) $(builddir)/$@.in >$@
