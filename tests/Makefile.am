AM_CPPFLAGS = -I$(top_srcdir)

TESTS_ENVIRONMENT = top_builddir=$(top_builddir)

TESTS = \
	test_file \
	test_store-retrieve \
	test_walk \
	test_iterator \
	$(NULL)

check_PROGRAMS = \
	test_file \
	test_store-retrieve \
	test_walk \
	test_iterator \
	$(NULL)

test_file_SOURCES = \
	test_file.c \
	utils.c \
	$(NULL)
test_file_LDADD = $(top_builddir)/datrie/libdatrie.la

test_store_retrieve_SOURCES = \
	test_store-retrieve.c \
	utils.c \
	$(NULL)
test_store_retrieve_LDADD = $(top_builddir)/datrie/libdatrie.la

test_walk_SOURCES = \
	test_walk.c \
	utils.c \
	$(NULL)
test_walk_LDADD = $(top_builddir)/datrie/libdatrie.la

test_iterator_SOURCES = \
	test_iterator.c \
	utils.c \
	$(NULL)
test_iterator_LDADD = $(top_builddir)/datrie/libdatrie.la

