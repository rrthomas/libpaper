ACLOCAL_AMFLAGS=-I m4

SUBDIRS = lib src man

dist_doc_DATA = README

EXTRA_DIST = m4/gnulib-cache.m4

release: distcheck
	git diff --exit-code && \
	git tag -a -m "Release tag" "v$(VERSION)" && \
	git push && git push --tags && \
	woger github package=$(PACKAGE) version=$(VERSION) dist_type=tar.gz
