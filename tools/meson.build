trietool = executable(
	'trietool',
	'trietool.c',
	dependencies : deps,
	link_with : datrie,
	install : true,
	include_directories : configuration_inc,
)

meson.add_install_script(sh, '-c', 'rm "$MESON_INSTALL_DESTDIR_PREFIX/bin/trietool-0.2" || true')
meson.add_install_script(sh, '-c', 'ln -s "$MESON_INSTALL_DESTDIR_PREFIX/bin/trietool" "$MESON_INSTALL_DESTDIR_PREFIX/bin/trietool-0.2"')
