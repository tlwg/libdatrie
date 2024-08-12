if not have_locale_charset and not have_langinfo_codeset
    subdir_done()
endif

trietool = executable(
    'trietool',
    'trietool.c',
    dependencies: deps,
    link_with: datrie,
    install: true,
    include_directories: configuration_inc,
)
meson.override_find_program('trietool', trietool)

install_symlink(
    'trietool-0.2',
    pointing_to: 'trietool',
    install_dir: get_option('bindir'),
)
