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

static _GL_ATTRIBUTE_CONST int paperdone(void) {
    struct paper *q;
    for (struct paper *p = papers; p; p = q) {
        char *s = (char *)p->name;
        free(s);
        q = p->next;
        free(p);
    }
    papers = NULL;
    return 0;
}

static _GL_ATTRIBUTE_PURE const char* papername(const struct paper* spaper)
{
    return spaper->name;
}

static _GL_ATTRIBUTE_PURE double paperpswidth(const struct paper* spaper)
{
    return spaper->pswidth;
}

static _GL_ATTRIBUTE_PURE double paperpsheight(const struct paper* spaper)
{
    return spaper->psheight;
}

static _GL_ATTRIBUTE_PURE const struct paper* paperfirst(void) {
    return papers;
}

static _GL_ATTRIBUTE_PURE const struct paper* papernext(const struct paper* spaper)
{
    assert(spaper);
    return spaper->next;
}

static _GL_ATTRIBUTE_PURE const struct paper* paperwithsize(double pswidth, double psheight)
{
    const struct paper* pp;

    for (pp = paperfirst(); pp; pp = papernext(pp))
        if (paperpswidth(pp) == pswidth && paperpsheight(pp) == psheight)
            return pp;

    return NULL;
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

#define PT_TO_MM(v) (unsigned int)((v * 2.54 * 10 / 72) + 0.5)

    const struct paper* pp;
    unsigned int w = NL_PAPER_GET(_NL_PAPER_WIDTH);
    unsigned int h = NL_PAPER_GET(_NL_PAPER_HEIGHT);
    for (pp = paperfirst(); pp; pp = papernext(pp))
        if (PT_TO_MM(pp->pswidth) == w && PT_TO_MM(pp->psheight) == h)
            return pp->name;
#endif

    return NULL;
}

static const char* systempapername(void) {
    char* paperstr = NULL;
    const struct paper* pp;
    char *paperenv = getenv("PAPERSIZE");

    if (paperenv)
        paperstr = strdup(paperenv);
    else {
        struct stat statbuf;
        const char* paperconf = getenv("PAPERCONF");
        if (!paperconf) paperconf = relocate(PAPERCONF);
        if (paperconf && stat(paperconf, &statbuf) == 0) {
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

        if (!paperstr) {
            const char *s = localepapername();
            if (s == NULL)
                s = PAPERSIZE;
            paperstr = strdup(s);
        }
    }

    if (paperstr && (pp = paperinfo(paperstr)))
        return strcpy(paperstr, pp->name);
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
	printf("%s", papername(paper));
	pr = 1;
    }

    if (options & OPT_SIZE) {
	if (pr) putchar(' ');
        printf("%g %g", paperpswidth(paper) / dim, paperpsheight(paper) / dim);
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
	for (const struct paper* p = paperfirst(); p; p = papernext(p))
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

    assert(paperdone() == 0);

    return ((options & OPT_ALL) || pinfo) ? EXIT_SUCCESS : 2;
}
