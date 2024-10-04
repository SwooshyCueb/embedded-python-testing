#!/usr/bin/env python3

import sys
import sysconfig
import site
import textwrap
from pprint import PrettyPrinter
from collections import OrderedDict

class UndefinedObject:
	def __repr__(self):
		return '<Undefined>'

undefined = UndefinedObject()

info = OrderedDict()

sys_dir = dir(sys)

modules = OrderedDict([
	(sys, [
		'version',
		'base_exec_prefix',
		'_base_executable',
		'base_prefix',
		'exec_prefix',
		'executable',
		'prefix',
		'path',
		'platlibdir',
		'_stdlib_dir',
		'_home',
	]),
	(site, [
		'PREFIXES',
		'USER_SITE',
		'USER_BASE',
	]),
	(sysconfig, [
		'_BASE_EXEC_PREFIX',
		'_BASE_PREFIX',
		'_EXEC_PREFIX',
		'_HAS_USER_BASE',
		'_INSTALL_SCHEMES',
		'_PREFIX',
		'_PROJECT_BASE',
		'_PYTHON_BUILD',
		'_SCHEME_KEYS',
		'_USER_BASE',
		'_sys_home',
	]),
])

sysconfig_vars = [
	'abiflags',
	'base',
	'BINDIR',
	'BINLIBDEST',
	'BUILDEXE',
	'BUILDPYTHON',
	'datarootdir',
	'DESTDIRS',
	'DESTLIB',
	'DESTPATH',
	'DESTSHARED',
	'exec_prefix',
	'EXE',
	'EXENAME',
	'EXPORTSFROM',
	'EXPORTSYMS',
	'installed_base',
	'installed_platbase',
	'platbase',
	'INSTSONAME',
	'LDLIBRARY',
	'LDLIBRARYDIR',
	'LDVERSION',
	'LIBDEST',
	'LIBDIR',
	'LIBPL',
	'LIBPYTHON',
	'LIBRARY',
	'LIBRARY_DEPS',
	'LINK_PYTHON_DEPS',
	'LOCALMODLIBS',
	'MACHDESTLIB',
	'MODLIBS',
	'PLATLIBDIR',
	'platlibdir',
	'prefix',
	'projectbase',
	'PY3LIBRARY',
	'PYTHON',
	'PYTHONPATH',
	'py_version_short',
	'SCRIPTDIR',
	'SITEPATH',
	'srcpath',
	'userbase',
	'WHEEL_PKG_DIR',
]

for module, variables in modules.items():
	mname = module.__name__
	for var in variables:
		vname = mname + '.' + var
		vval = getattr(module, var, undefined)
		info[vname] = vval

install_scehemes = info['sysconfig._INSTALL_SCHEMES']
info['sysconfig._INSTALL_SCHEMES'] = {name: v for name, v in install_scehemes.items() if not name.startswith('nt') and not name.startswith('osx')}

for sysconfig_var in sysconfig_vars:
	sysconfig_val = sysconfig.get_config_var(sysconfig_var)
	if sysconfig_val is None:
		sysconfig_val = undefined
	info['sysconfig[' + sysconfig_var + ']'] = sysconfig_val

try:
	preferred_schemes = sysconfig._get_preferred_schemes()
except AttributeError:
	pass
else:
	for layout, scheme in preferred_schemes.items():
		info['preferred ' + layout + ' scheme'] = scheme

scheme_names = [name for name in sysconfig.get_scheme_names() if not name.startswith('nt') and not name.startswith('osx')]
try:
	default_scheme = sysconfig.get_default_scheme()
except AttributeError:
	default_scheme = sysconfig._get_default_scheme()
scheme_names.remove(default_scheme)
scheme_names.insert(0, default_scheme)

info['default path scheme'] = default_scheme

for scheme in scheme_names:
	scheme_paths = sysconfig.get_paths(scheme, expand=True)
	info[scheme + ' paths'] = scheme_paths

title_max_len = 0
for name in info:
	title_max_len = max(title_max_len, len(name))

title_max_len = title_max_len + 2
pp_w = 120 - title_max_len

pp = PrettyPrinter(indent=2, width=pp_w, compact=False)

for name, value in info.items():
	name_len = len(name) + 1
	entry = textwrap.indent(pp.pformat(value), ' ' * title_max_len)
	print(name + ':' + entry[name_len:])

print('\n\n\n\n')
