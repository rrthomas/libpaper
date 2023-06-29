/* Copyright (C) 1996, Yves Arrouye <arrouye@debian.org>
                 2001, Adrian Bunk <bunk@fs.tum.de>
                 2023, Reuben Thomas <rrt@sc3d.org>

   This program is distributed under the GNU General Public Licence version
   2. See https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
*/

#include "config.h"

#include <sys/param.h>

#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <paper.h>

static void usage(const char* name)
{
    fprintf(stderr,
	    "usage: %s [ [ -p ] papername | -d | -a ] [ -z ] [ -n | -N ] [ -s | -w | -h ] [ -c | -m | -i ]\n",
	    name);
    exit(1);
}

#define OPT_NAME	1
#define OPT_UPPERNAME	2
#define OPT_WIDTH	4
#define OPT_HEIGHT      8
#define OPT_CM         16
#define OPT_MM         32
#define OPT_INCH       64
#define OPT_CONTINUE  128

#define OPT_UNIT      (OPT_CM | OPT_MM | OPT_INCH)

#define INCHxCM 2.54

static void printf_downcase(const char *s)
{
    if (strlen(s) == 0)
	return;
    printf("%c%s", tolower(s[0]), s + 1);
}

static void printinfo(const struct paper* paper, int options)
{
    int pr = 0;

    if ((options & ~(OPT_CONTINUE)) == 0)
	options = OPT_NAME;

    if (options & OPT_NAME) {
	printf_downcase(papername(paper));
	pr = 1;
    } else if (options & OPT_UPPERNAME) {
	if (islower(*papername(paper)))
	    printf("%c%s", toupper(*papername(paper)), papername(paper) + 1);
	pr = 1;
    }

    if (options & OPT_WIDTH) {
	if (pr)
	    putchar(' ');
        if (options & OPT_CM)
	    printf("%g cm", paperpswidth(paper) / 72.0 * INCHxCM );
	else if (options & OPT_MM)
	    printf("%g mm", paperpswidth(paper) / 72.0 * 10 * INCHxCM );
	else if (options & OPT_INCH)
	    printf("%g\"", paperpswidth(paper) / 72.0 );
	else
	    printf("%g", paperpswidth(paper) );
	pr = 1;
    }
    if (options & OPT_HEIGHT) {
	if (pr) putchar(' ');
        if (options & OPT_CM)
	    printf("%g cm", paperpsheight(paper) / 72.0 * INCHxCM );
	else if (options & OPT_MM)
	    printf("%g mm", paperpsheight(paper) / 72.0 * 10 * INCHxCM );
	else if (options & OPT_INCH)
	    printf("%g\"", paperpsheight(paper) / 72.0 );
	else
	    printf("%g", paperpsheight(paper) );
	pr = 1;
    }

    putchar('\n');
}

int main(int argc, char** argv)
{
    int all = 0;
    const char* paper = NULL;
    int options = 0;

    const char* progname = strrchr(*argv, '/');
    if (progname)
	++progname;
    else
	progname = *argv;

    int c;
    while ((c = getopt(argc, argv, "adznNswhcmip:")) != EOF) {
	switch (c) {
	case 'a':
	    if (paper || all)
		usage(progname);
	    all = 1;
	    break;
	case 'd':
	    if (paper || all)
		usage(progname);
	    paper = defaultpapername();
	    break;
	case 'p':
	    if (paper || all)
		usage(progname);
	    paper = optarg;
	    break;
	case 'z':
	    options |= OPT_CONTINUE;
	    break;
	case 'n':
	    if (options & OPT_UPPERNAME) usage(progname);
	    options |= OPT_NAME;
	    break;
	case 'N':
	    if (options & OPT_NAME) usage(progname);
	    options |= OPT_UPPERNAME;
	    break;
	case 's':
	    options |= OPT_WIDTH | OPT_HEIGHT;
	    break;
	case 'w':
	    options |= OPT_WIDTH;
	    break;
	case 'h':
	    options |= OPT_HEIGHT;
	    break;
	case 'c':
	    if (options & OPT_UNIT) usage(progname);
	    options |= OPT_CM;
	    break;
	case 'm':
	    if (options & OPT_UNIT) usage(progname);
	    options |= OPT_MM;
	    break;
	case 'i':
	    if (options & OPT_UNIT) usage(progname);
	    options |= OPT_INCH;
	    break;
	default:
	    usage(progname);
	}
    }

    if (optind < argc - 1 || (paper && optind != argc))
	usage(progname);
    else if (optind != argc)
	paper = argv[optind];

    paperinit();

    if (all) {
 	const struct paper* papers;

	for (papers = paperfirst(); papers; papers = papernext(papers))
	    printinfo(papers, options);
    } else {
        const struct paper* syspaper;

        if (!paper)
	    paper = systempapername();
        if (!paper)
	    paper = defaultpapername();
	if (!paper) {
	    char errmsg[2 * MAXPATHLEN + 64];
	    sprintf(errmsg, "%s: cannot get system paper size", progname);

	    if (errno)
		perror(errmsg);
	    else
	        fputs(errmsg, stderr);

	    paperdone();
	    exit(3);
	}

        syspaper = paperinfo(paper);

        if (syspaper)
            printinfo(syspaper, options);
        else {
	    fprintf(stderr, "%s: unknown paper `%s'\n", progname, paper);
	    if (options & OPT_CONTINUE)
		printf_downcase(paper);

	    paperdone();
	    exit(2);
        }
    }

    paperdone();
    return 0;
}
