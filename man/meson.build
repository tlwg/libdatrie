install_man('trietool.1')

meson.add_install_script(sh, '-c', 'cd ${MESON_INSTALL_DESTDIR_PREFIX}/@0@/man1 && ln -s trietool.1 trietool-0.2.1 || true'.format(get_option('mandir')))
