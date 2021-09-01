project('datrie', 'c', version : '0.2.13') # TODO: How to get this??
conf_data = configuration_data()

conf_data.set_quoted('VERSION', meson.project_version())

# TODO: Doxygen

# LT_CURRENT=5
# LT_REVISION=0
# LT_AGE=4

compiler = meson.get_compiler('c')

deps = []

compiler_has_iconv = compiler.has_function('iconv_open')
iconv_pkgconfig = get_option('iconv_pkgconfig')
if iconv_pkgconfig.auto()
	if not compiler_has_iconv
		deps += dependency('iconv')
	endif
elif iconv_pkgconfig.enabled()
	deps += dependency('iconv')
elif iconv_pkgconfig.disabled()
	if not compiler_has_iconv
		error('No usable iconv() implementation found')
	endif
endif

have_locale_charset = compiler.has_function('locale_charset', dependencies : deps)
if have_locale_charset
	conf_data.set('HAVE_LOCALE_CHARSET', 1)
endif

have_langinfo_codeset = compiler.compiles('''
#include <langinfo.h>
void func() { char *codeset = nl_langinfo (CODESET); }
''', name : 'nl_langinfo (CODESET)')
if have_langinfo_codeset
	conf_data.set('HAVE_LANGINFO_CODESET', 1)
endif

if not have_locale_charset and not have_langinfo_codeset
	error('No locale_charset() nor nl_langinfo(CODESET) found.\nPlease consider installing GNU libiconv')
endif

# LD_HAS_VERSION_SCRIPT? whether linker supports -version-script

configure_file(output : 'config.h', configuration: conf_data)
configuration_inc = include_directories('.')

# Subdir: man doc tests
subdir('datrie')
subdir('tools')

# FIXME: Dist
