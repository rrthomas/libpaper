AM_CFLAGS = $(WERROR_CFLAGS) $(WARN_CFLAGS)

lib_LTLIBRARIES = libpaper.la
libpaper_la_SOURCES = dimen.c paper.c dimen.h
libpaper_la_LDFLAGS = -version-info 3:0:0
libpaper_la_CPPFLAGS = -I$(top_srcdir)/lib -I$(top_builddir)/lib -D_REENTRANT -DPAPERCONF='"$(PAPERCONF)"' -DRUNPARTSDIR='"$(RUNPARTSDIR)"' -DPAPERSPECS='"$(PAPERSPECS)"'

include_HEADERS = paper.h

sysconf_DATA = paperspecs

EXTRA_DIST = paperspecs
