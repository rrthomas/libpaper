dnl Process this file with autoconf to produce a configure script

AC_INIT([paper], [2.0])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall])

dnl Check for standard build environment
AC_GNU_SOURCE
AC_PROG_CC
gl_EARLY
gl_INIT
AC_PATH_PROG(PERL, perl)

dnl set up some substitutions
PAPERCONF="papersize"
AC_SUBST(PAPERCONF)
AH_TEMPLATE(PAPERCONF, [Default paper size configuration file name])

PAPERSPECS="paperspecs"
AC_SUBST(PAPERSPECS)
AH_TEMPLATE(PAPERSPECS, [Paper specifications configuration file name])

PERL_EXEEXT=.pl
AC_SUBST(PERL_EXEEXT)
AH_TEMPLATE(PERL_EXEEXT, [File extension for Perl scripts on systems that use EXEEXT])

dnl Create files
AC_CONFIG_HEADER([config.h])
AC_OUTPUT([
	Makefile
	lib/Makefile
	src/Makefile
        src/paper.in
	man/Makefile
	man/paper.1.in
        man/paperspecs.5.in
])
