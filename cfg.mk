# Configuration for maintainer-makefile

local-checks-to-skip = \
	sc_bindtextdomain \
	sc_immutable_NEWS

# Rationale:
#
# sc_bindtextdomain: libpaper isn't internationalised
# sc_immutable_NEWS: we don't have a proper NEWS
