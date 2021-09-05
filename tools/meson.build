if not have_locale_charset and not have_langinfo_codeset
    subdir_done()
endif

trietool = executable(
    'trietool',
    'trietool.c',
    dependencies : deps,
    link_with : datrie,
    install : true,
    include_directories : configuration_inc,
)

bindir_abs = get_option('prefix') + '/' + get_option('bindir')
if sh.found()
    meson.add_install_script(sh, '-c', 'cd @0@ && ln -s trietool trietool-0.2 || true'.format(bindir_abs))
endif
