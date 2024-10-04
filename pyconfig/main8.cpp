#include <fmt/format.h>
#include <fmt/xchar.h>

#include <Python.h>
#include <osdefs.h>

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <string>

const bool isolated_config = false;

const char* python_script = PYEMBED_PLAYGROUND_PATH "/venv-test/print-deets.py";

int main(int argc, char* argv[]) {
	int ret;

	Py_InitializeEx(0);
	
	FILE* fp = std::fopen(python_script, "r");
	ret = PyRun_SimpleFileEx(fp, python_script, 1);

	Py_Finalize();

	return ret;
}
