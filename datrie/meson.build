ldflags = [
    '-Wl,-no-undefined',
]
if ld_has_version_script
    ldflags += [
        '-Wl,--version-script,@0@'.format(meson.current_source_dir() / 'libdatrie.map')
    ]
endif

datrie_src = files(
    'alpha-map-private.h',
    'alpha-map.c',
    'alpha-map.h',
    'darray.c',
    'darray.h',
    'dstring-private.h',
    'dstring.c',
    'dstring.h',
    'fileutils.c',
    'fileutils.h',
    'tail.c',
    'tail.h',
    'trie-private.h',
    'trie-string.c',
    'trie-string.h',
    'trie.c',
    'trie.h',
    'triedefs.h',
    'typedefs.h',
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
    'alpha-map.h',
    'trie.h',
    'triedefs.h',
    'typedefs.h',
    subdir : 'datrie',
)

pkg = import('pkgconfig')
pkg.generate(
    datrie,
    description : 'Double-array trie library',
    url : 'https://github.com/tlwg/libdatrie',
    version : version.stdout(),
)
