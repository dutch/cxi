ACLOCAL_AMFLAGS = -I m4
AM_MAKEFLAGS = --no-print-directory
EXTRA_DIST = $(top_srcdir)/.version
BUILT_SOURCES = $(top_srcdir)/.version

$(top_srcdir)/.version:
	$(AM_V_GEN) echo $(VERSION) >$@.tmp && mv $@.tmp $@

dist-hook:
	echo $(VERSION) >$(distdir)/.tarball-version

include $(srcdir)/src/Makefile.inc
