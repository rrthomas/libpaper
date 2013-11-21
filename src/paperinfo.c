/*
 * Copyright (C) Yves Arrouye <Yves.Arrouye@marin.fdn.fr>, 1996.
 * Copyright (C) Reuben Thomas <rrt@sc3d.org>, 2013.
 *
 * Use under the GPL version 2. You are not allowed to remove this
 * copyright notice.
 *
 */

#include <config.h>

#include <stdbool.h>
#include <locale.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#if defined LC_PAPER && defined _GNU_SOURCE
#include <langinfo.h>
#endif

#include "progname.h"
#include "relocatable.h"

static struct {
    const char *name;
    double factor;
} units[] = {
    { "pt",	1. },
    { "mm",	72. * .1 / 2.54 },
    { "in",	72. },
    { 0 }
};

static _GL_ATTRIBUTE_PURE double unitfactor(const char *unit)
{
    for (int i = 0; units[i].name; ++i)
        if (!strcasecmp(units[i].name, unit))
            return units[i].factor;

    return 0;
}

/* Return next token from a string.
   On the first call, p points to the string, and saveptr is ignored;
   on subsequent calls, p should be NULL, and saveptr unchanged since
   the previous call.
   NULL is returned when no token is available. */
static char *gettok(char *p, char **saveptr) {
    if (p == NULL)
        p = *saveptr;
    for (; *p && isspace(*p); p++)
        ;
    size_t i = 0;
    for (; p[i] && !isspace(p[i]); i++)
        ;
    *saveptr = p + i;
    if (i == 0)
        return NULL;
    return strndup(p, i);
}

struct paper {
    const char *name;
    double pswidth, psheight;
    struct paper *next;
};

struct paper *papers;
const char *paperspecs, *paperconf;

static _GL_ATTRIBUTE_CONST int paperinit(void) {
    paperspecs = relocate(PAPERSPECS);
    paperconf = relocate(PAPERCONF);

    int ret = 0;
    FILE *ps;
    if ((ps = fopen(paperspecs, "r"))) {
        struct paper *prev = NULL, *p;
        size_t n;
        char *l;
        for (l = NULL; ret == 0 && getline(&l, &n, ps) > 0; prev = p) {
            char *saveptr;
            char *name = gettok(l, &saveptr);
            char *wstr = gettok(NULL, &saveptr), *hstr = gettok(NULL, &saveptr);
            char *unit = gettok(NULL, &saveptr);
            if (name && wstr && hstr && unit) {
                errno = 0;
                double w = strtod(wstr, NULL);
                if (errno) ret = 1;
                errno = 0;
                double h = strtod(hstr, NULL);
                if (errno) ret = 1;
                double dim;
                if ((dim = unitfactor(unit)) == 0)
                    ret = 1;
                else {
                    w *= dim;
                    h *= dim;
                }
                p = calloc(1, sizeof(struct paper));
                if (!p) ret = -1;
                else {
                    *p = (struct paper){name, w, h, NULL};
                    if (prev)
                        prev->next = p;
                    else
                        prev = papers = p;
                }
            } else
                ret = 2;
            free(wstr);
            free(hstr);
            free(unit);
        }
        fclose(ps);
        free(l);
    } else
        ret = -1;

    return ret;
}

static _GL_ATTRIBUTE_PURE const struct paper *paperinfo(const char *paper)
{
    for (struct paper *p = papers; p; p = p->next)
        if (strcasecmp(paper, p->name) == 0)
            return p;

    return NULL;
}

static const char *localepapername(void) {
#if defined LC_PAPER && defined _GNU_SOURCE

#define NL_PAPER_GET(x)         \
  ((union { char *string; unsigned word; })nl_langinfo(x)).word
#define MM_TO_PT(v) (unsigned)((v * 72 / 2.54 / 10) + 0.5)
    unsigned w = MM_TO_PT(NL_PAPER_GET(_NL_PAPER_WIDTH));
    unsigned h = MM_TO_PT(NL_PAPER_GET(_NL_PAPER_HEIGHT));
    for (struct paper *pp = papers; pp; pp = pp->next)
        if ((unsigned)(pp->pswidth + 0.5) == w && (unsigned)(pp->psheight + 0.5) == h)
            return pp->name;
#endif

    return NULL;
}

static const char *readpaperconf(const char *file) {
    char *paperstr = NULL;
    FILE *ps;
    if ((ps = fopen(file, "r"))) {
        char *l = NULL, *saveptr = NULL;
        size_t n;
        if (getline(&l, &n, ps) > 0)
            paperstr = gettok(l, &saveptr);

        free(l);
        fclose(ps);
    }
    return paperstr;
}

static const char *systempapername(void) {
    const char *paperstr = getenv("PAPERSIZE");
    if (!paperstr) {
        const char *file = getenv("PAPERCONF");
        if (file)
            paperstr = readpaperconf(file);
    }
    if (!paperstr)
        paperstr = localepapername();
    if (!paperstr)
        paperstr = readpaperconf(paperconf);
    if (!paperstr)
        paperstr = papers->name;

    const struct paper *pp = paperinfo(paperstr);
    return pp ? pp->name : paperstr;
}

static void usage(const char *name)
{
    fprintf(stderr,
            "Usage: %s [-n] [-s] [-u UNIT] [-a|PAPER...]\n",
            name);
    exit(EXIT_FAILURE);
}

bool opt_name, opt_size, opt_unit, opt_all;

static void printinfo(const struct paper *paper, double dim)
{
    int pr = 0;

    if (opt_name || opt_all || !opt_size) {
        printf("%s", paper->name);
        pr = 1;
    }

    if (opt_size) {
        if (pr) putchar(' ');
        printf("%g %g", paper->pswidth / dim, paper->psheight / dim);
    }

    putchar('\n');
}

int main(int argc, char *argv[])
{
    set_program_name(argv[0]);
    setlocale(LC_ALL, "");

    const char *unit = NULL;
    double dim = 1.0;
    int c;
    while ((c = getopt(argc, argv, "ansu:")) != EOF) {
        switch (c) {
        case 'a':
            opt_all = true;
            break;

        case 'n':
            opt_name = true;
            break;

        case 's':
            opt_size = true;
            break;

        case 'u':
            opt_unit = true;
            unit = optarg;
            dim = unitfactor(unit);
            if (dim == 0) usage(program_name);
            break;

        default:
            usage(program_name);
        }
    }

    if (opt_all && optind != argc)
        usage(program_name);

    const struct paper *pinfo = NULL;
    if (paperinit())
        fprintf(stderr, "%s: could not read `%s'\n", program_name, paperspecs);
    else {
        if (opt_all) {
            for (const struct paper *p = papers; p; p = p->next)
                printinfo(p, dim);
        } else {
            if (optind < argc - 1) opt_name = true;
            else if (optind == argc) printinfo(paperinfo(systempapername()), dim);
            for (int i = optind; i < argc; i++) {
                if ((pinfo = paperinfo(argv[i])))
                    printinfo(pinfo, dim);
                else {
                    fprintf(stderr, "%s: unknown paper `%s'\n", program_name, argv[i]);
                    break;
                }
            }
        }
    }

    return (opt_all || pinfo) ? EXIT_SUCCESS : 2;
}
