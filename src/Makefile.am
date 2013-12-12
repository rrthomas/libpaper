libexec_PROGRAMS= localepaper
LDADD		= $(top_builddir)/lib/libgnu.a
AM_CPPFLAGS	= -I$(top_srcdir)/lib

bin_SCRIPTS	= paper
sysconf_DATA	= paperspecs

EXTRA_DIST	= $(sysconf_DATA)

edit = sed -e 's|$$[{]prefix[}]|$(prefix)|g'

paper: $(builddir)/paper.in
	$(edit) $(builddir)/$@.in >$@
