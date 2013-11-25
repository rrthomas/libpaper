man_MANS = \
	paperinfo.1 \
	paperspecs.5

EXTRA_DIST = \
	paperinfo.1.in.in \
	paperspecs.5.in.in

DISTCLEANFILES = $(man_MANS)

edit = sed -e 's|$$[{]prefix[}]|$(prefix)|g'

paperinfo.1: $(builddir)/paperinfo.1.in
	$(edit) $(builddir)/$@.in >$@

paperspecs.5: $(builddir)/paperspecs.5.in
	$(edit) $(builddir)/$@.in >$@
