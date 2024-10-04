#include <fmt/format.h>
#include <fmt/xchar.h>

#include <Python.h>
#include <osdefs.h>

#include <cstdio>

#ifndef PY_VER_WITH_ABI
#define PY_VER_WITH_ABI BOOST_STRINGIZE(PY_MAJOR_VERSION) "." BOOST_STRINGIZE(PY_MINOR_VERSION)
#endif

const bool isolated_config = false;

int main(int argc, char* argv[]) {
	PyStatus py_status;
	PyPreConfig py_preconfig;
	PyConfig py_config;

	if (isolated_config) {
		PyPreConfig_InitIsolatedConfig(&py_preconfig);
	}
	else {
		PyPreConfig_InitPythonConfig(&py_preconfig);
	}

	py_status = Py_PreInitialize(&py_preconfig);
	if (PyStatus_Exception(py_status)) {
		fmt::println("Failed to preinitialize Python interpreter with status [{0}]: {1}", py_status.exitcode, py_status.err_msg);
		return -1;
	}

	if (isolated_config) {
		PyConfig_InitIsolatedConfig(&py_config);
	}
	else {
		PyConfig_InitPythonConfig(&py_config);
	}

	py_config.pythonpath_env = PYEMBED_PLAYGROUND_PATH_L L"/pyconfig/site-packages:" PYEMBED_PLAYGROUND_PATH_L L"/venv-test";

	py_status = Py_InitializeFromConfig(&py_config);
	if (PyStatus_Exception(py_status)) {
		fmt::println("Failed to initialize Python interpreter with status [{0}]: {1}", py_status.exitcode, py_status.err_msg);
		return -1;
	}

	for (Py_ssize_t i = 0; i < py_config.module_search_paths.length; ++i) {
		fmt::println(fmt::runtime(py_config.module_search_paths.items[i]));
	}

	const char* fn = PYEMBED_PLAYGROUND_PATH "/venv-test/print-deets.py";
	FILE* fp = std::fopen(fn, "r");
	return PyRun_SimpleFileEx(fp, fn, 1);


//	PyRun_SimpleString(R"PY(
//import sys
//from pprint import pprint
//pprint(sys.path)
//	)PY");

	//return 0;
}
