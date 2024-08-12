if get_option('docs').disabled()
    subdir_done()
endif

doxygen = find_program('doxygen', required : get_option('docs'), version : '>=1.9.1')
if not doxygen.found()
    subdir_done()
endif

doxyfile_data = configuration_data()
doxyfile_data.set('VERSION', version)
doxyfile_data.set('PACKAGE', meson.project_name())
doxyfile_data.set('top_builddir', meson.project_build_root())
doxyfile_data.set('top_srcdir', meson.project_source_root())

doxyfile = configure_file(
    input : 'Doxyfile.in',
    output : 'Doxyfile',
    configuration : doxyfile_data,
)

custom_target(
    'docs',
    input : doxyfile,
    output : 'html',
    command : [doxygen, doxyfile],
    depend_files : datrie_src,
    install : true,
    install_dir: get_option('docsdir'),
)
