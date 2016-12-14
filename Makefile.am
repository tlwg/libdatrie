ACLOCAL_AMFLAGS = -I m4

SUBDIRS = datrie tools man doc tests

EXTRA_DIST =	\
	README.migration		\
	build-aux/git-version-gen	\
	$(NULL)

doc_DATA = README.migration

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = datrie-0.2.pc

dist-hook:
	echo $(VERSION) > $(distdir)/VERSION
