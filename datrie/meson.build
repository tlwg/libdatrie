ldflags = [
	'-Wl,-no-undefined',
]
if ld_has_version_script
	ldflags += [
		'-Wl,--version-script,@0@'.format(meson.current_source_dir() / 'libdatrie.map')
	]
endif

datrie_src = files(
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
)

datrie = library(
	'datrie', datrie_src,
	
	dependencies : deps,
	install : true,
	link_args : ldflags,
	link_depends : ['libdatrie.map', 'libdatrie.def'],
	version : meson.project_version(),
	include_directories : configuration_inc,
)

install_headers(
	'typedefs.h',
	'triedefs.h',
	'alpha-map.h',
	'trie.h',
	subdir : 'datrie',
)

pkg = import('pkgconfig')
pkg.generate(
	datrie,
	description : 'Double-array trie library',
	url : 'https://github.com/tlwg/libdatrie',
	version : version.stdout(),
)
