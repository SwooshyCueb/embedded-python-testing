#include <fmt/format.h>
#include <fmt/xchar.h>

#include <Python.h>
#include <osdefs.h>

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <string>

#define IRODS_STRINGIZE(X) IRODS_DO_STRINGIZE(X)
#define IRODS_DO_STRINGIZE(X) #X

#define IRODS_JOIN(X, Y) IRODS_DO_JOIN(X, Y)
#define IRODS_DO_JOIN(X, Y) IRODS_DO_JOIN2(X,Y)
#define IRODS_DO_JOIN2(X, Y) X##Y

#ifndef PY_VER_WITH_ABI
#define PY_VER_WITH_ABI IRODS_STRINGIZE(PY_MAJOR_VERSION) "." IRODS_STRINGIZE(PY_MINOR_VERSION)
#endif

const bool isolated_config = false;

#define VENV_ROOT PYEMBED_PLAYGROUND_PATH "/venv-test/venv-" PY_VER_WITH_ABI
const char* python_script = PYEMBED_PLAYGROUND_PATH "/venv-test/print-deets.py";

int main(int argc, char* argv[]) {
	int ret;

	const char* path_env_cstr = getenv("PATH");
	const std::string path_env_orig(path_env_cstr);
	const std::string venv_bindir(VENV_ROOT "/bin");
	const std::string path_env_str(venv_bindir + ":" + path_env_orig);

	ret = setenv("VIRTUAL_ENV", VENV_ROOT, 1);
	if (ret != 0) {
		fmt::println("Failed to set environment variable VIRTUAL_ENV: [{0}] {1}", errno, std::strerror(errno));
	}
	ret = setenv("PATH", path_env_str.c_str(), 1);
	if (ret != 0) {
		fmt::println("Failed to set environment variable PATH: [{0}] {1}", errno, std::strerror(errno));
	}


	Py_InitializeEx(0);
	
	FILE* fp = std::fopen(python_script, "r");
	ret = PyRun_SimpleFileEx(fp, python_script, 1);

	Py_Finalize();

	return ret;
}
