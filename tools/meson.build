trietool = executable(
    'trietool',
    'trietool.c',
    dependencies : deps,
    link_with : datrie,
    install : true,
    include_directories : configuration_inc,
)

bindir_abs = '$DESTDIR' + get_option('prefix') + '/' + get_option('bindir')
meson.add_install_script(sh, '-c', 'cd @0@ && ln -s trietool trietool-0.2 || true'.format(bindir_abs))
