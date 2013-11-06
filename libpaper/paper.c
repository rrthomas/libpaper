
/*
 * Copyright (C) Yves Arrouye <Yves.Arrouye@marin.fdn.fr>, 1996.
 *
 * Use under the GPL version 2. You are not allowed to remove this
 * copyright notice.
 *
 */

#include <config.h>

#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>

#include <locale.h>
#include <langinfo.h>

#include "paper.h"

struct paper {
    const char* name;
    double pswidth, psheight;
};

/* This table will be searched in order: please put typical paper sizes
   at the beginning of it. */

/* The paper names have been got from gs 3.68 gs_stadt.ps file, with
   some personal additions. */

static struct paper papers[] = {
#include "paperspecs.h"
    { 0 }
};

int paperinit(void) {
    return 0;
}

int paperdone(void) {
    return 0;
}

const char* papername(const struct paper* spaper)
{
    return spaper->name;
}

double paperpswidth(const struct paper* spaper)
{
    return spaper->pswidth;
}

double paperpsheight(const struct paper* spaper)
{
    return spaper->psheight;
}

const struct paper* paperfirst(void) {
    return papers;
}

const struct paper* paperlast(void) {
    static const struct paper* lastpaper = 0;
   
    const struct paper* next = papers;
    while (next->name) {
	lastpaper = next, ++next;
    }

    return lastpaper;
}

const struct paper* papernext(const struct paper* spaper)
{
    return (++spaper)->name ? spaper : 0;
}

const struct paper* paperprev(const struct paper* spaper)
{
    return spaper == papers ? 0 : --spaper;
}

const char* defaultpapersizefile(void) {
    return PAPERCONF;
}

const char* systempapersizefile(void) {
    const char* paperconf = getenv(PAPERCONFVAR);
/* 
Previously PAPERCONFVAR used to contain a paper name and PAPERSIZEVAR
contained a file path.  Now they're reversed.  If we don't find a '/'
in PAPERCONFVAR, fall-back to the old behaviour.
*/

    if ((paperconf != NULL) && (strchr(paperconf, '/') == NULL)) {
	paperconf = getenv(PAPERSIZEVAR);
	if ((paperconf != NULL) && (strchr(paperconf, '/') == NULL))
	    paperconf = NULL;
    }

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

    for (pp = paperfirst(); pp; pp = papernext(pp)) {
	if (
             PT_TO_MM(pp->pswidth) == w &&
             PT_TO_MM(pp->psheight) == h
           ) {
	    return pp->name;
	}
    }
#endif
    return PAPERSIZE;
}

char* systempapername(void) {
    const char* paperconf;
    char* paperstr;
    char* paperenv;
    const char* paperdef;
    FILE* ps;
    struct stat statbuf;
    const struct paper* pp;
    int c;

/* 
Previously PAPERSIZEVAR used to contain a file path and PAPERCONFVAR
contained a paper name.  Now they're reversed.  If we find a '/' in
PAPERSIZEVAR, fall-back to the old behaviour.
*/

    paperenv = getenv(PAPERSIZEVAR);
    if ((paperenv != NULL) && (strchr(paperenv, '/') != NULL)) {
	paperenv = getenv(PAPERCONFVAR);
	if ((paperenv != NULL) && (strchr(paperenv, '/') != NULL))
	    paperenv = NULL;
    }

    if (paperenv) {
        paperstr = malloc((strlen(paperenv) + 1) * sizeof(char));	
	
	if (! paperstr) return 0;
	
	if ((pp = paperinfo(paperenv)))
	    return strcpy(paperstr, pp->name);
	else
	    return strcpy(paperstr, paperenv);
    }

    paperconf = systempapersizefile();
    if (paperconf && stat(paperconf, &statbuf) == -1) return 0;
    
    if (!paperconf) paperconf = defaultpapersizefile();

    if ((stat(paperconf, &statbuf) != -1) &&
	(ps = fopen(paperconf, "r"))) {

        while ((c = getc(ps)) != EOF) {
	    if (c == '#') {
	        while ((c = getc(ps)) != EOF && c != '\n');
		if (c == EOF) {
		    break;
		}
	    } else if (!isspace(c)) {
	        unsigned n = 0, m = 64;
		char* papername = malloc(m * sizeof(char));
	    
		if (!papername) {
		    fclose(ps);
		    return 0;
		}
	    
		do {
		    if (n == m-1) {
		        char* newpaper = realloc(papername,
						 (m *= 2) * sizeof(char));
			if (!newpaper) {
			    free(papername);
			    fclose(ps);
			    return 0;
			}
			papername = newpaper;
		    }
		    papername[n++] = c;
		} while ((c = getc(ps)) != EOF && !isspace(c));

		papername[n] = 0;

		fclose(ps);

		paperstr = malloc((strlen(papername) + 1) * sizeof(char));
		if (! paperstr) return 0;

		strcpy(paperstr, papername);
		free(papername);

		if ((pp = paperinfo(paperstr)))
		    return strcpy(paperstr, pp->name);
		else
		    return paperstr;
	    }
	}
    } 
      
    paperdef = defaultpapername();
    paperstr = malloc((strlen(paperdef) + 1) * sizeof(char));
    
    if (paperstr) 
        return strcpy(paperstr, paperdef); 
    else
        return 0;
}

const struct paper* paperinfo(const char* paper)
{
    const struct paper* pp;

    for (pp = paperfirst(); pp; pp = papernext(pp)) {
	if (!strcasecmp(pp->name, paper)) {
	    return pp;
	}
    }

    return 0;
}

const struct paper* paperwithsize(double pswidth, double psheight)
{
    const struct paper* pp;

    for (pp = paperfirst(); pp; pp = papernext(pp)) {
	if (pp->pswidth == pswidth
	    && pp->psheight == psheight) {
	    return pp;
	}
    }

    return 0;
}

