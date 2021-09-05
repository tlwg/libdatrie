if get_option('docs').disabled()
    subdir_done()
endif

required_doxygen_version = '>=1.9.1'

doxygen = find_program('doxygen', required : get_option('docs'))
if not doxygen.found()
    subdir_done()
endif
doxygen_version = run_command(doxygen, '--version')
if not doxygen_version.stdout().version_compare(required_doxygen_version)
    message = 'Doxygen version @0@ is under requirements @1@'.format(doxygen_version, required_doxygen_version)
    if get_option('docs').enabled()
        error(message)
    else
        warning(message)
        subdir_done()
    endif
endif

doxyfile_data = configuration_data()
doxyfile_data.set('VERSION', version)
doxyfile_data.set('PACKAGE', meson.project_name())
doxyfile_data.set('top_builddir', meson.build_root())
doxyfile_data.set('top_srcdir', meson.source_root())

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
