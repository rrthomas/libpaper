AM_CFLAGS = $(WERROR_CFLAGS) $(WARN_CFLAGS)

bin_PROGRAMS	= paperinfo
paperinfo_LDADD		= $(top_builddir)/libpaper/libpaper.la $(top_builddir)/lib/libgnu.la
paperinfo_CPPFLAGS	= $(ISYSTEM)$(top_srcdir)/lib $(ISYSTEM)$(top_srcdir)/libpaper -DINSTALLDIR=\"$(bindir)\"
if RELOCATABLE_VIA_LD
paperinfo_LDFLAGS	= `$(RELOCATABLE_LDFLAGS) $(bindir)`
endif

bin_SCRIPTS	= paperconfig

EXTRA_DIST	= paperconfig.in
