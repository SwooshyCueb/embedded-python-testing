#include <fmt/format.h>

#include <boost/python.hpp>

#include <codecvt>
#include <cstddef>
#include <locale>
#include <string>
#include <vector>

namespace bp = boost::python;

const bool isolated_config = false;

std::string extract_python_exception(void)
{
	PyObject *exc, *val, *tb;
	PyErr_Fetch(&exc, &val, &tb);
	PyErr_NormalizeException(&exc, &val, &tb);
	bp::handle<> hexc(exc), hval(bp::allow_null(val)), htb(bp::allow_null(tb));

	if (!hval) {
		return bp::extract<std::string>(bp::str(hexc));
	}
	else {
		bp::object traceback(bp::import("traceback"));
		bp::object format_exception(traceback.attr("format_exception"));
		bp::object formatted_list(format_exception(hexc, hval, htb));
		bp::object formatted(bp::str("").join(formatted_list));

		return bp::extract<std::string>(formatted);
	}
}

int main(int argc, char* argv[]) {
	PyStatus py_status;
	PyPreConfig py_preconfig_tmp;
	PyConfig py_config_tmp;

	if (isolated_config) {
		PyPreConfig_InitIsolatedConfig(&py_preconfig_tmp);
	}
	else {
		PyPreConfig_InitPythonConfig(&py_preconfig_tmp);
	}

	py_preconfig_tmp.parse_argv = 0;
	py_preconfig_tmp.utf8_mode = 1;

	py_status = Py_PreInitialize(&py_preconfig_tmp);
	if (PyStatus_Exception(py_status)) {
		fmt::println("Failed to preinitialize Python interpreter with status [{0}]: {1}", py_status.exitcode, py_status.err_msg);
		return -1;
	}

	if (isolated_config) {
		PyConfig_InitIsolatedConfig(&py_config_tmp);
	}
	else {
		PyConfig_InitPythonConfig(&py_config_tmp);
	}

	py_config_tmp.parse_argv = 0;
#if PY_VERSION_HEX < 0x030B0000
	py_config_tmp.safe_path = 1;
#endif
	py_config_tmp.install_signal_handlers = 0;

	py_status = Py_InitializeFromConfig(&py_config_tmp);
	if (PyStatus_Exception(py_status)) {
		fmt::println("Failed to initialize Python interpreter with status [{0}]: {1}", py_status.exitcode, py_status.err_msg);
		return -1;
	}

	std::vector<std::wstring> module_search_paths;

	try {
		bp::object mod_sys = bp::import("sys");
		auto sys_path = mod_sys.attr("path");
		bp::list sys_path_list = bp::extract<bp::list>(sys_path);
		std::size_t sys_path_len = bp::extract<std::size_t>(sys_path_list.attr("__len__")());
		module_search_paths.reserve(sys_path_len);
		fmt::println("Default paths:");
		for (std::size_t sys_path_idx = 0; sys_path_idx < sys_path_len; ++sys_path_idx) {
			auto sys_path_entry = sys_path_list[sys_path_idx];
			bp::extract<std::wstring> sys_path_entry_ex{sys_path_entry};
			if (sys_path_entry_ex.check()) {
				std::wstring sys_path_entry_wstr = sys_path_entry_ex();
				fmt::println(" - {0}", std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(sys_path_entry_wstr));
				module_search_paths.push_back(sys_path_entry_wstr);
			}
		}
	}
	catch (const bp::error_already_set&) {
		const std::string formatted_python_exception = extract_python_exception();
		fmt::println(fmt::runtime(formatted_python_exception));
	}

	PyPreConfig py_preconfig;
	PyConfig py_config;

	if (isolated_config) {
		PyPreConfig_InitIsolatedConfig(&py_preconfig);
	}
	else {
		PyPreConfig_InitPythonConfig(&py_preconfig);
	}

	py_preconfig.parse_argv = 0;
	py_preconfig.utf8_mode = 1;

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

	py_config.parse_argv = 0;
#if PY_VERSION_HEX < 0x030B0000
	py_config.safe_path = 1;
#endif
	py_config.install_signal_handlers = 0;

	for (auto&& module_search_path : module_search_paths) {
		PyWideStringList_Append(&py_config.module_search_paths, module_search_path.c_str());
	}
	//PyWideStringList_Append(&py_config.module_search_paths, PYEMBED_PLAYGROUND_PATH_L L"/pyconfig/site-packages");
	//py_config.module_search_paths_set = 1;

	py_status = Py_InitializeFromConfig(&py_config);
	if (PyStatus_Exception(py_status)) {
		fmt::println("Failed to initialize Python interpreter with status [{0}]: {1}", py_status.exitcode, py_status.err_msg);
		return -1;
	}

	try {
		{
			bp::object mod_sys = bp::import("sys");
			auto sys_path = mod_sys.attr("path");
			bp::list sys_path_list = bp::extract<bp::list>(sys_path);
			sys_path_list.append(bp::str(PYEMBED_PLAYGROUND_PATH "/pyconfig/site-packages"));
		}
		bp::object mod_sys = bp::import("sys");
		auto sys_path = mod_sys.attr("path");
		bp::list sys_path_list = bp::extract<bp::list>(sys_path);
		std::size_t sys_path_len = bp::extract<std::size_t>(sys_path_list.attr("__len__")());
		fmt::println("Paths:");
		for (std::size_t sys_path_idx = 0; sys_path_idx < sys_path_len; ++sys_path_idx) {
			auto sys_path_entry = sys_path_list[sys_path_idx];
			bp::extract<std::wstring> sys_path_entry_ex{sys_path_entry};
			if (sys_path_entry_ex.check()) {
				std::wstring sys_path_entry_wstr = sys_path_entry_ex();
				fmt::println(" - {0}", std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(sys_path_entry_wstr));
			}
		}
	}
	catch (const bp::error_already_set&) {
		const std::string formatted_python_exception = extract_python_exception();
		fmt::println(fmt::runtime(formatted_python_exception));
	}

	return 0;
}
