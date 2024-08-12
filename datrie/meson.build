ldflags = []
link_depends = ['libdatrie.map']

if ld_has_version_script
    ldflags += [
        '-Wl,--version-script,@0@'.format(
            meson.current_source_dir() / 'libdatrie.map',
        ),
    ]
endif

if compiler.get_linker_id() in ['link', 'lld-link']
    link_def = custom_target(
        'link-def',
        input: ['libdatrie.def'],
        # This use ASCII as some PS version do not have UTF8NoBOM
        command: [
            powershell,
            '-Command',
            '''& {
            Write-Output "EXPORTS" | Out-File -Encoding ascii -FilePath @OUTPUT0@
            Get-Content -Path @INPUT0@ | Out-File -Append -Encoding ascii -FilePath @OUTPUT0@
        }''',
        ],
        output: 'libdatrie.def',
    )
    ldflags += ['/def:@0@'.format(link_def.full_path())]
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
    'datrie',
    datrie_src,
    dependencies: deps,
    install: true,
    link_args: ldflags,
    link_depends: link_depends,
    version: libversion,
    soversion: soversion,
    include_directories: configuration_inc,
)
libdatrie_dep = declare_dependency(
    include_directories: configuration_inc,
    link_with: datrie,
)
meson.override_dependency('datrie-0.2', libdatrie_dep)
meson.override_dependency('libdatrie', libdatrie_dep)

install_headers(
    'alpha-map.h',
    'trie.h',
    'triedefs.h',
    'typedefs.h',
    subdir: 'datrie',
)

pkg = import('pkgconfig')
pkg.generate(
    datrie,
    filebase: 'datrie-0.2',
    description: 'Double-array trie library',
    url: 'https://github.com/tlwg/libdatrie',
    version: version,
)
