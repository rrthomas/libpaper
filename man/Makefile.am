man_MANS = \
	paperconf.1 \
	libpaper.3 \
	paperconfig.8

EXTRA_DIST = \
	$(man_MANS) \
	paperconf.1.in.in \
	libpaper.3.in.in \
	paperconfig.8.in.in

DISTCLEANFILES = $(generated_MANS)

edit = sed -e 's|$$[{]prefix[}]|$(prefix)|g'

paperconf.1: $(builddir)/paperconf.1.in
	$(edit) $(builddir)/$@.in >$@

libpaper.3: $(builddir)/libpaper.3.in
	$(edit) $(builddir)/$@.in >$@

paperconfig.8: $(builddir)/paperconfig.8.in
	$(edit) $(builddir)/$@.in >$@
