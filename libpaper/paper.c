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
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>

#include <locale.h>
#if defined LC_PAPER  && defined _GNU_SOURCE
#include <langinfo.h>
#endif

#include "hash.h"

#include "paper.h"
#include "dimen.h"

/* Return a line that contains non-blank non-comment characters.
   A comment is any line whose first non-blank is a hash */
static char *gettokline(FILE *fp) {
    char *l = NULL;
    size_t sz;
    ssize_t n;

    for (n = 0; (n = getline(&l, &sz, fp)) != -1;) {
        char *p;
        for (p = l; isspace(*p); p++)
            ;
        if (*p != '\0' && *p != '#')
            return l;
    }

    free(l);
    return NULL;
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
};

static size_t paperhash(const void *v, size_t n) {
    return hash_string(((const struct paper *)v)->name, n);
}

static bool papereq(const void *v, const void *w) {
    return strcasecmp(((const struct paper *)v)->name, ((const struct paper *) w)->name) == 0;
}

static void paperfree(void *v) {
    char *s = (char *)((struct paper *)v)->name;
    free(s);
    free(v);
}

Hash_table *papers;

_GL_ATTRIBUTE_CONST int paperinit(void) {
    int ret = 0;

    papers = hash_initialize(256, NULL, paperhash, papereq, paperfree);

    FILE *ps;
    if ((ps = fopen(PAPERSPECS, "r"))) {
        for (char *l = NULL; ret == 0 && (l = gettokline(ps));) {
            char *saveptr;
            char *name = gettok(l, &saveptr);
            char *wstr = gettok(NULL, &saveptr), *hstr = gettok(NULL, &saveptr);
            char *unit = gettok(NULL, &saveptr);
            free(l);
            if (name && wstr && hstr) {
                errno = 0;
                double w = strtod(wstr, NULL);
                if (errno) ret = 1;
                errno = 0;
                double h = strtod(hstr, NULL);
                if (errno) ret = 1;
                if (unit) {
                    float dim;
                    if ((dim = unitfactor(unit)) == 0)
                        ret = 1;
                    else {
                        w *= (double)dim;
                        h *= (double)dim;
                    }
                }
                struct paper *p = malloc(sizeof(struct paper));
                if (!p) ret = -1;
                else *p = (struct paper){name, w, h};
                if (hash_insert(papers, p) == NULL)
                    ret = -1;
            } else
                ret = 2;
            free(wstr);
            free(hstr);
            free(unit);
        }
        fclose(ps);
    } else
        ret = -1;

    return ret;
}

_GL_ATTRIBUTE_CONST int paperdone(void) {
    assert(papers);
    hash_free(papers);
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
    assert(papers);
    return hash_get_first(papers);
}

_GL_ATTRIBUTE_PURE const struct paper* papernext(const struct paper* spaper)
{
    assert(papers);
    return hash_get_next(papers, spaper);
}

_GL_ATTRIBUTE_CONST const char* defaultpapersizefile(void) {
    return PAPERCONF;
}

const char* systempapersizefile(void) {
    const char* paperconf = getenv(PAPERCONFVAR);
    return paperconf ? paperconf : defaultpapersizefile();
}

const char* defaultpapername(void) {

#if defined LC_PAPER  && defined _GNU_SOURCE
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

    return PAPERSIZE;
}

char* systempapername(void) {
    char* paperstr = NULL;
    const struct paper* pp;
    char *paperenv = getenv(PAPERSIZEVAR);

    if (paperenv)
        paperstr = strdup(paperenv);
    else {
        struct stat statbuf;
        FILE* ps;
        const char *paperconf = systempapersizefile();

        if (paperconf && stat(paperconf, &statbuf) == -1) return 0;

        if (!paperconf) paperconf = defaultpapersizefile();

        if ((ps = fopen(paperconf, "r"))) {
            char *l = gettokline(ps), *saveptr;

            if (l)
                paperstr = gettok(l, &saveptr);

            free(l);
            fclose(ps);
        } else
            paperstr = strdup(defaultpapername());
    }

    if (paperstr && (pp = paperinfo(paperstr)))
        return strcpy(paperstr, pp->name);
    return paperstr;
}

_GL_ATTRIBUTE_PURE const struct paper* paperinfo(const char* paper)
{
    struct paper p = {paper, 0, 0};
    assert(papers);
    return hash_lookup(papers, &p);
}

_GL_ATTRIBUTE_PURE const struct paper* paperwithsize(double pswidth, double psheight)
{
    const struct paper* pp;

    for (pp = paperfirst(); pp; pp = papernext(pp))
	if (pp->pswidth == pswidth && pp->psheight == psheight)
	    return pp;

    return NULL;
}
