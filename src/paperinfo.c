/*
 * Copyright (C) Yves Arrouye <Yves.Arrouye@marin.fdn.fr>, 1996.
 * Copyright (C) Reuben Thomas <rrt@sc3d.org>, 2013.
 *
 * Use under the GPL version 2. You are not allowed to remove this
 * copyright notice.
 *
 */

#include <config.h>

#include <assert.h>
#include <sys/stat.h>
#include <locale.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#if defined LC_PAPER  && defined _GNU_SOURCE
#include <langinfo.h>
#endif

#include "progname.h"
#include "relocatable.h"

static struct {
    const char* name;
    double factor;
} units[] = {
    { "pt",	1. },
    { "mm",	72. * .1 / 2.54 },
    { "in",	72. },
    { 0 }
};

static _GL_ATTRIBUTE_PURE double unitfactor(const char* unit)
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
    const char* name;
    double pswidth, psheight;
    struct paper *next;
};

struct paper *papers;

static _GL_ATTRIBUTE_CONST int paperinit(void) {
    int ret = 0;
    FILE *ps;
    if ((ps = fopen(relocate(PAPERSPECS), "r"))) {
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

static _GL_ATTRIBUTE_PURE const struct paper* paperinfo(const char* paper)
{
    for (struct paper *p = papers; p; p = p->next)
        if (strcasecmp(paper, p->name) == 0)
            return p;

    return NULL;
}

static const char* localepapername(void) {
#if defined LC_PAPER && defined _GNU_SOURCE

#define NL_PAPER_GET(x)         \
  ((union { char *string; unsigned int word; })nl_langinfo(x)).word
#define MM_TO_PT(v) (unsigned int)((v * 72 / 2.54 / 10) + 0.5)
    double w = MM_TO_PT(NL_PAPER_GET(_NL_PAPER_WIDTH));
    double h = MM_TO_PT(NL_PAPER_GET(_NL_PAPER_HEIGHT));
    for (struct paper *pp = papers; pp; pp = pp->next)
        if (floor(pp->pswidth + 0.5) == w && floor(pp->psheight + 0.5) == h)
            return pp->name;
#endif

    return NULL;
}

static const char* systempapername(void) {
    const char* paperstr = NULL;
    const struct paper* pp;
    char *paperenv = getenv("PAPERSIZE");

    if (paperenv)
        paperstr = paperenv;
    else {
        struct stat statbuf;
        const char* paperconf = getenv("PAPERCONF");
        if (!paperconf)
            paperstr = localepapername();
        if (!paperstr) {
            paperconf = relocate(PAPERCONF);
            if (stat(paperconf, &statbuf) == 0) {
                FILE* ps;
                if ((ps = fopen(paperconf, "r"))) {
                    char *l = NULL, *saveptr = NULL;
                    size_t n;
                    if (getline(&l, &n, ps) > 0)
                        paperstr = gettok(l, &saveptr);

                    free(l);
                    fclose(ps);
                }
            }
        }
        if (paperstr == NULL)
            paperstr = PAPERSIZE;
    }

    if (paperstr && (pp = paperinfo(paperstr)))
        return pp->name;
    return paperstr;
}

static void usage(const char* name)
{
    fprintf(stderr,
            "Usage: %s [-n] [-s] [-u UNIT] [-a|PAPER...]\n",
	    name);
    exit(EXIT_FAILURE);
}

#define OPT_NAME	1
#define OPT_SIZE	2
#define OPT_UNIT        4
#define OPT_ALL         8

static void printinfo(const struct paper* paper, int options, double dim)
{
    int pr = 0;

    if (options & (OPT_NAME | OPT_ALL) || options == 0) {
	printf("%s", paper->name);
	pr = 1;
    }

    if (options & OPT_SIZE) {
	if (pr) putchar(' ');
        printf("%g %g", paper->pswidth / dim, paper->psheight / dim);
    }

    putchar('\n');
}

int main(int argc, char** argv)
{
    set_program_name(argv[0]);
    setlocale(LC_ALL, "");

    const char *unit = NULL;
    double dim = 1.0;
    int c;
    unsigned options = 0;
    while ((c = getopt(argc, argv, "ansu:")) != EOF) {
	switch (c) {
        case 'a':
            options |= OPT_ALL;
            break;

        case 'n':
            options |= OPT_NAME;
            break;

        case 's':
            options |= OPT_SIZE;
            break;

        case 'u':
            options |= OPT_UNIT;
            unit = optarg;
            dim = unitfactor(unit);
            if (dim == 0) usage(program_name);
            break;

        default:
            usage(program_name);
	}
    }

    if (((options & OPT_ALL) && optind != argc) || (!(options & OPT_ALL) && optind == argc && argc > 1))
	usage(program_name);

    assert(paperinit() == 0);

    const struct paper *pinfo = NULL;
    if (options & OPT_ALL) {
	for (const struct paper* p = papers; p; p = p->next)
	    printinfo(p, options, dim);
    } else {
        if (optind < argc - 1) options |= OPT_NAME;
        for (int i = optind; i < argc; i++) {
            if ((pinfo = paperinfo(argv[i])))
                printinfo(pinfo, options, dim);
            else {
                fprintf(stderr, "%s: unknown paper `%s'\n", program_name, argv[i]);
                break;
            }
        }
        if (argc == 1)
            printinfo(paperinfo(systempapername()), options, dim);
    }

    return ((options & OPT_ALL) || pinfo) ? EXIT_SUCCESS : 2;
}
