project(
    'libdatrie',
    'c',
    meson_version: '>=1.0.0',
    version: run_command(files('build-aux/git-version-gen'), check: true).stdout().strip(),
)

fs = import('fs')

sh = find_program('sh')
compiler = meson.get_compiler('c')
powershell = find_program('powershell', required: compiler.get_id() == 'msvc')
deps = [dependency('iconv', required: false)]

version = run_command(files('build-aux/git-version-gen'), check: true).stdout().strip()
meson.add_dist_script(
    sh,
    '-c',
    'echo "@0@" > "$MESON_DIST_ROOT/VERSION"'.format(version),
)
meson.add_dist_script(sh, '-c', 'cd "$MESON_DIST_ROOT" && ./autogen.sh')
meson.add_dist_script(
    sh,
    '-c',
    'cd "$MESON_DIST_ROOT" && rm -r autom4te.cache autogen.sh || true',
)

LT_CURRENT = 5
LT_REVISION = 0
LT_AGE = 4

soversion = LT_CURRENT - LT_AGE
libversion = '@0@.@1@.@2@'.format(soversion, LT_AGE, LT_REVISION)

conf_data = configuration_data()
conf_data.set_quoted('VERSION', version)

# Detect for locale_charset
have_locale_charset = compiler.has_function('locale_charset', dependencies: deps)
if have_locale_charset
    conf_data.set('HAVE_LOCALE_CHARSET', 1)
endif

# Detect for nl_langinfo(CODESET)
have_langinfo_codeset = compiler.compiles(
    '''
#include <langinfo.h>
void func() { char *codeset = nl_langinfo (CODESET); }
''',
    name: 'nl_langinfo (CODESET)',
)
if have_langinfo_codeset
    conf_data.set('HAVE_LANGINFO_CODESET', 1)
endif

if not have_locale_charset and not have_langinfo_codeset
    warning(
        'No locale_charset() nor nl_langinfo(CODESET) found. Please consider installing GNU libiconv',
    )
endif

# Detect for version-script support
ld_has_version_script = compiler.has_link_argument(
    '-Wl,-version-script,@0@'.format(
        meson.project_source_root() / 'build-aux' / 'test.map',
    ),
)

# Generate config.h
configure_file(output: 'config.h', configuration: conf_data)
configuration_inc = include_directories('.')

# Build!
subdir('datrie')
subdir('tests')
subdir('tools')
subdir('man')
subdir('doc')
