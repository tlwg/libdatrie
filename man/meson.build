install_man('trietool.1')

mandir_abs = get_option('prefix') / get_option('mandir')
meson.add_install_script(sh, '-c', 'cd @0@ && ln -s trietool.1 trietool-0.2.1 || true'.format(mandir_abs / 'man1'))
