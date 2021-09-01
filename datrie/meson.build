datrie = library(
	'datrie',

	'typedefs.h',
	'triedefs.h',
	'trie-private.h',
	'dstring.h',
	'dstring-private.h',
	'dstring.c',
	'trie-string.h',
	'trie-string.c',
	'fileutils.h',
	'fileutils.c',
	'darray.h',
	'darray.c',
	'tail.h',
	'tail.c',
	'trie.h',
	'trie.c',
	'alpha-map.h',
	'alpha-map-private.h',
	'alpha-map.c',
	
	dependencies : deps,
	install : true,
)

install_headers(
	'typedefs.h',
	'triedefs.h',
	'alpha-map.h',
	'trie.h',
	subdir : 'datrie',
)
# TODO: gen pkgconfig

# if LD_HAS_VERSION_SCRIPT
# EXPORTS_FLAGS = -Wl,-version-script -Wl,$(srcdir)/libdatrie.map
# EXPORTS_DEPS = libdatrie.map
# else
# EXPORTS_FLAGS = -export-symbols $(srcdir)/libdatrie.def
# EXPORTS_DEPS = libdatrie.def
# endif

# libdatrie_la_LDFLAGS =	-no-undefined \
# 	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
# 	$(EXPORTS_FLAGS)
