ldflags = []
link_depends = ['libdatrie.map']

if ld_has_version_script
    ldflags += [
        '-Wl,--version-script,@0@'.format(meson.current_source_dir() / 'libdatrie.map')
    ]
endif

if compiler.get_id() == 'msvc'
    link_def = custom_target(
        'link-def',
        input : ['libdatrie.def'],
        command : [powershell, '-Command', '''& {
            Write-Output "EXPORTS" > @OUTPUT0@
            Get-Content -Path @INPUT0@ >> @OUTPUT0@
        }'''],
        output : 'libdatrie.def',
    )
    ldflags += [ '/def:@0@'.format(link_def.full_path()) ]
    link_depends += [link_def]
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
    link_depends : link_depends,
    version : libversion,
    soversion : soversion,
    include_directories : configuration_inc,
)
libdatrie_dep = declare_dependency(
    include_directories : configuration_inc,
    link_with : datrie
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
    filebase : 'datrie-0.2',
    description : 'Double-array trie library',
    url : 'https://github.com/tlwg/libdatrie',
    version : version,
)
