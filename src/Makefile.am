AM_CFLAGS = $(WERROR_CFLAGS) $(WARN_CFLAGS)

bin_PROGRAMS	= paperinfo
paperinfo_LDADD		= $(top_builddir)/lib/libgnu.a
paperinfo_CPPFLAGS	= $(ISYSTEM)$(top_srcdir)/lib -DINSTALLDIR=\"$(bindir)\" -DPAPERSPECS='"$(PAPERSPECS)"'
if RELOCATABLE_VIA_LD
paperinfo_LDFLAGS	= `$(RELOCATABLE_LDFLAGS) $(bindir)`
endif

sysconf_DATA	= paperspecs

EXTRA_DIST	= $(sysconf_DATA)
