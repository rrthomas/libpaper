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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <locale.h>
#if defined LC_PAPER  && defined _GNU_SOURCE
#include <langinfo.h>
#endif

#include "relocatable.h"

#include "paper.h"

static struct {
    const char* name;
    double factor;
} units[] = {
    { "pt",	1. },
    { "mm",	72. * .1 / 2.54 },
    { "in",	72. },
    { 0 }
};

_GL_ATTRIBUTE_PURE double unitfactor(const char* unit)
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

_GL_ATTRIBUTE_CONST int paperinit(void) {
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

_GL_ATTRIBUTE_CONST int paperdone(void) {
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

_GL_ATTRIBUTE_PURE const char* papername(const struct paper* spaper)
{
    return spaper->name;
}

_GL_ATTRIBUTE_PURE double paperpswidth(const struct paper* spaper)
{
    return spaper->pswidth;
}

_GL_ATTRIBUTE_PURE double paperpsheight(const struct paper* spaper)
{
    return spaper->psheight;
}

_GL_ATTRIBUTE_PURE const struct paper* paperfirst(void) {
    return papers;
}

_GL_ATTRIBUTE_PURE const struct paper* papernext(const struct paper* spaper)
{
    assert(spaper);
    return spaper->next;
}

static const char* systempapersizefile(void) {
    const char* paperconf = getenv("PAPERCONF");
    return paperconf ? paperconf : relocate(PAPERCONF);
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

const char* systempapername(void) {
    char* paperstr = NULL;
    const struct paper* pp;
    char *paperenv = getenv("PAPERSIZE");

    if (paperenv)
        paperstr = strdup(paperenv);
    else {
        struct stat statbuf;
        const char *paperconf = systempapersizefile();
        if (paperconf && stat(paperconf, &statbuf) == 0) {
            FILE* ps;
            if ((ps = fopen(paperconf, "r"))) {
                char *l = NULL, *saveptr;
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

_GL_ATTRIBUTE_PURE const struct paper* paperinfo(const char* paper)
{
    for (struct paper *p = papers; p; p = p->next)
        if (strcasecmp(paper, p->name) == 0)
            return p;

    return NULL;
}

_GL_ATTRIBUTE_PURE const struct paper* paperwithsize(double pswidth, double psheight)
{
    const struct paper* pp;

    for (pp = paperfirst(); pp; pp = papernext(pp))
        if (pp->pswidth == pswidth && pp->psheight == psheight)
            return pp;

    return NULL;
}
