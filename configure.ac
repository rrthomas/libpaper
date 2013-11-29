dnl Process this file with autoconf to produce a configure script.

AC_INIT([paper], [2.0])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

dnl Check for standard build environment
gl_EARLY
AC_PROG_CC
AC_GNU_SOURCE
AC_PROG_INSTALL

gl_INIT

dnl Perl
AX_WITH_PROG([PERL], [perl])
if test -z "$PERL"; then
  AC_MSG_FAILURE([cannot find perl])
fi

dnl set up some substitutions
PAPERSPECS="paperspecs"
AC_SUBST(PAPERSPECS)
AH_TEMPLATE(PAPERSPECS, [Paper specifications configuration file location])

dnl Create files
AC_CONFIG_HEADER([config.h])
AC_OUTPUT([
	Makefile
	lib/Makefile
	src/Makefile
        src/paperinfo
	man/Makefile
	man/paperinfo.1.in
        man/paperspecs.5.in
])
