/*
  Paper sizes command-line utility.

  Copyright (c) 2021-2022 Reuben Thomas <rrt@sc3d.org>.

  This file is part of libpaper.

  This program is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at
  your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "config.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <locale.h>

#include "progname.h"
#include "quote.h"
#include "relocatable.h"
#include "xvasprintf.h"

#include "paper.h"


#define PROGRAM_NAME "paper"
const char *unit_list;
char *unit_msg;

/* Options table. */
static struct option longopts[] = {
#define O(longname, shortname, arg, argstring, docstring) \
  {longname, arg, NULL, shortname},
#include "tbl_opts.h"
#undef O
  {0, 0, 0, 0}
};

static void usage(int exit_code)
{
    char *buf;
    fprintf(stderr, "Usage: %s [OPTION...] [PAPER...|--all]\n"
            "\n"
            "Print paper size information.\n"
            "\n",
            program_name);
#define O(longname, shortname, arg, argstring, docstring)       \
    buf = xasprintf("  --%s %s", longname, argstring);          \
    fprintf(stderr, "%-20s%s\n", buf, docstring);               \
    free(buf);
#include "tbl_opts.h"
#undef O
    fprintf(stderr, "\n"
            "Report bugs at " PACKAGE_URL "\n");
    exit(exit_code);
}

enum paper_unit explicit_unit = PAPER_UNIT_INVALID;
bool opt_size;

static void printinfo(const struct paper *paper)
{
    enum paper_unit unit = paperunit(paper);
    double dim = 1.0;
    const char *unitname = paperunitname(unit);
    if (explicit_unit != PAPER_UNIT_INVALID) {
        dim = paperunitfactor(explicit_unit) / paperunitfactor(unit);
        assert(dim != 0.0);
        unitname = paperunitname(explicit_unit);
    }

    printf("%s", papername(paper));
    if (opt_size)
        printf(": %gx%g %s", paperwidth(paper) / dim, paperheight(paper) / dim, unitname);

    putchar('\n');
}

_GL_ATTRIBUTE_FORMAT_PRINTF_STANDARD(1, 2) static void die(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "%s: ", program_name);
    vfprintf(stderr, fmt, ap);
    putc('\n', stderr);
    exit(1);
}

static void paper_die(const char *msg)
{
    die("%s in line %zu of %s", msg, paper_lineno, paper_specsfile);
}

static void printpaper(const char *name) {
    const struct paper *paper = paperinfo(name);
    if (paper == NULL)
        die("unknown paper %s", quote(name));
    printinfo(paper);
}

int main(int argc, char **argv)
{
#if ENABLE_RELOCATABLE
    /* Set up libpaper relocation. */
    set_program_name_and_installdir(
#ifndef _WIN32
				    argv[0],
#else
				    "paper",
#endif
				    REAL_INSTALLPREFIX, INSTALLDIR);
    char *full_progname = get_full_program_name();
    char *curr_prefix = compute_curr_prefix(REAL_INSTALLPREFIX, INSTALLDIR, full_progname);
    papersetprefixdir(curr_prefix);
#endif

    setlocale(LC_ALL, "");

    /* Parse command-line options. */
    bool opt_all = false;
    opt_size = true;
    unit_list = "pt, mm, in"; // FIXME: generate this list
    unit_msg = xasprintf("unit in which to show sizes [%s; default: natural units]", unit_list);

    opterr = 0; /* Don't display errors for unknown options. */
    for (;;) {
        int this_optind = optind ? optind : 1, longindex = -1, c;

        /* Leading `:' so as to return `:' for a missing arg, not `?'. */
        c = getopt_long(argc, argv, ":", longopts, &longindex);

        if (c == -1) /* No more options. */
            break;
        else if (c == '?') /* Unknown option. */
            die("unknown option %s", quote(argv[this_optind]));
        else if (c == ':') /* Missing argument. */
            die("option %s requires an argument", quote(argv[this_optind]));

        switch (longindex) {
        case 0:
            opt_all = true;
            break;
        case 1:
            opt_size = false;
            break;
        case 2:
            explicit_unit = paperunitfromname(optarg);
            if (explicit_unit == PAPER_UNIT_INVALID)
                die("bad unit (valid units: %s)", unit_list);
            break;
        case 3:
            usage(EXIT_SUCCESS);
            break;
        case 4:
            printf(PROGRAM_NAME " " VERSION "\n"
                   "Copyright (c) 2013-2022 Reuben Thomas <rrt@sc3d.org>.\n"
                   PROGRAM_NAME " comes with ABSOLUTELY NO WARRANTY.\n"
                   "You may redistribute copies of " PROGRAM_NAME "\n"
                   "under the terms of the GNU General Public License\n"
                   "version 2, or, at your option, any later version."
                   "See <https://www.gnu.org/licenses/gpl-2.html>.\n");
            exit(EXIT_SUCCESS);
        default:
            break;
        }
    }

    /* Skip processed arguments. */
    argc -= optind;
    argv += optind;

    /* Check --all not used with explicit paper sizes. */
    if (opt_all && argc > 0)
        usage(1);

    /* Initialize libpaper. */
    int ret = paperinit();
    switch (ret) {
    case PAPER_OK:
        /* Do nothing. */
        break;
    case PAPER_BAD_WIDTH:
        paper_die("bad width");
        break;
    case PAPER_BAD_HEIGHT:
        paper_die("bad height");
        break;
    case PAPER_BAD_UNIT:
        paper_die("bad unit");
        break;
    case PAPER_MISSING_FIELD:
        paper_die("missing field");
        break;
    case PAPER_NOMEM:
        paper_die("out of memory");
        break;
    default:
        paper_die("unknown error");
        break;
    }
    const char *defaultname = defaultpapername();
    if (defaultname == NULL)
        die("no default paper size is set");

    if (opt_all) {
        for (const struct paper *p = paperfirst(); p; p = papernext(p))
            printinfo(p);
    } else {
        if (argc == 0)
            printpaper(defaultname);
        else
            for (int i = 0; i < argc; i++)
                printpaper(argv[i]);
    }

#if ENABLE_RELOCATABLE
    free(curr_prefix);
#endif

    exit(EXIT_SUCCESS);
}
