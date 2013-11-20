#include <config.h>

#include <locale.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <paper.h>

#include "progname.h"
#include "relocatable.h"

static void usage(const char* name)
{
    fprintf(stderr,
	"Usage: %s [[-p] PAPERNAME|-a] [-n] [-s|-w|-h] [-u UNIT]\n",
	    name);
    exit(EXIT_FAILURE);
}

#define OPT_NAME	1
#define OPT_WIDTH	2
#define OPT_HEIGHT      4
#define OPT_UNIT        8

static void printinfo(const struct paper* paper, int options, double dim, const char *unit)
{
    int pr = 0;

    if (options & OPT_NAME || options == 0) {
	printf("%s", papername(paper));
	pr = 1;
    }

    if (options & OPT_WIDTH) {
	if (pr) putchar(' ');
        if (options & OPT_UNIT)
          printf("%g %s", paperpswidth(paper) / dim, unit );
	else
          printf("%g", paperpswidth(paper) );
	pr = 1;
    }
    if (options & OPT_HEIGHT) {
	if (pr) putchar(' ');
        if (options & OPT_UNIT)
          printf("%g %s", paperpsheight(paper) / dim, unit );
	else
          printf("%g", paperpsheight(paper) );
    }

    putchar('\n');
}

int main(int argc, char** argv)
{
    set_program_name(argv[0]);
    setlocale(LC_ALL, "");

    const char *paper = NULL, *unit = NULL;
    double dim = 1.0;
    int c, all = 0;
    unsigned options = 0;
    while ((c = getopt(argc, argv, "answhp:u:")) != EOF) {
	switch (c) {
	    case 'a':
		if (paper)
		    usage(program_name);
		all = 1;
		break;

	    case 'p':
		if (paper || all)
		    usage(program_name);
		paper = optarg;
		break;

	    case 'n':
		options |= OPT_NAME;
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

    if (optind < argc - 1 || (paper && optind != argc)) {
	usage(program_name);
    } else if (optind != argc) {
	paper = argv[optind];
    }

    paperinit();

    const struct paper* syspaper = NULL;
    if (all) {
	for (const struct paper* papers = paperfirst(); papers; papers = papernext(papers))
	    printinfo(papers, options, dim, unit);
    } else {
        if (!paper) paper = systempapername();

        if ((syspaper = paperinfo(paper)))
            printinfo(syspaper, options, dim, unit);
        else
	    fprintf(stderr, "%s: unknown paper `%s'\n", program_name, paper);
    }

    paperdone();

    return (all || syspaper) ? EXIT_SUCCESS : 2;
}
