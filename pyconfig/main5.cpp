//#include <boost/python.hpp>
#include <Python.h>
#include <fmt/format.h>
#include <fmt/chrono.h>

#include <cstddef>
#include <chrono>
#include <ratio>
#include <thread>
#include <vector>

namespace
{

        constexpr std::size_t THREADS_QTY = 4;

        namespace python_state
        {
                static PyThreadState* ts_main;

                static thread_local PyThreadState* ts_thread;
                static thread_local PyThreadState* ts_thread_old;
                static thread_local uint64_t ts_thread_refct = 0;
        }

        struct python_tstate_scope
        {
                python_tstate_scope()
                {
                        if (python_state::ts_thread_refct == 0) {
                                python_state::ts_thread = PyThreadState_New(python_state::ts_main->interp);
                                PyEval_RestoreThread(python_state::ts_thread);
                                python_state::ts_thread_old = PyThreadState_Swap(python_state::ts_thread);
                        }
                        python_state::ts_thread_refct++;
                }

                ~python_tstate_scope()
                {
                        if (python_state::ts_thread_refct == 1) {
                                PyThreadState_Swap(python_state::ts_thread_old);
                                PyThreadState_Clear(python_state::ts_thread);
                                PyThreadState_DeleteCurrent();
                        }
                        python_state::ts_thread_refct--;
                }
        };

        struct python_gstate_scope
        {
                PyGILState_STATE gstate;

                python_gstate_scope()
                        : gstate(PyGILState_Ensure())
                {
                }

                ~python_gstate_scope()
                {
                        PyGILState_Release(gstate);
                }
        };
}

void f_tstate()
{
        python_tstate_scope tstate{};

        {
                python_tstate_scope tstate2{};
                {
                        //boost::python::str s{"core"};
                        PyObject* s = PyUnicode_FromString("core");
                        Py_DECREF(s);
                }
        }
}

void f_gstate()
{
        python_gstate_scope gstate{};

        {
                python_gstate_scope gstate2{};
                {
                        //boost::python::str s{"core"};
                        PyObject* s = PyUnicode_FromString("core");
                        Py_DECREF(s);
                }
        }
}

void test_tstate() {
        std::vector<std::thread> v;
        v.reserve(THREADS_QTY);

        f_tstate();
        for (std::size_t i = 0; i < THREADS_QTY; ++i) {
                v.emplace_back(f_tstate);
        }
        for (std::size_t i = 0; i < THREADS_QTY; ++i) {
                v[i].join();
        }
        f_tstate();
}

void test_gstate() {
        std::vector<std::thread> v;
        v.reserve(THREADS_QTY);

        f_gstate();
        for (std::size_t i = 0; i < THREADS_QTY; ++i) {
                v.emplace_back(f_gstate);
        }
        for (std::size_t i = 0; i < THREADS_QTY; ++i) {
                v[i].join();
        }
        f_gstate();
}

constexpr std::size_t ST_TEST_RUNS = 5000000;
constexpr std::size_t MT_TEST_RUNS =   10000;

int main()
{
        Py_InitializeEx(0);
        
        python_state::ts_main = PyEval_SaveThread();

        auto st_tstate_ts_before = std::chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < ST_TEST_RUNS; ++i) {
                f_tstate();
        }
        auto st_tstate_ts_after = std::chrono::high_resolution_clock::now();

        auto st_gstate_ts_before = std::chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < ST_TEST_RUNS; ++i) {
                f_gstate();
        }
        auto st_gstate_ts_after = std::chrono::high_resolution_clock::now();

        auto mt_tstate_ts_before = std::chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < MT_TEST_RUNS; ++i) {
                test_tstate();
        }
        auto mt_tstate_ts_after = std::chrono::high_resolution_clock::now();

        auto mt_gstate_ts_before = std::chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < MT_TEST_RUNS; ++i) {
                test_gstate();
        }
        auto mt_gstate_ts_after = std::chrono::high_resolution_clock::now();
        
        PyEval_RestoreThread(python_state::ts_main);

        std::chrono::duration<double, std::milli> st_tstate_ms_dur = st_tstate_ts_after - st_tstate_ts_before;
        std::chrono::duration<double, std::milli> st_gstate_ms_dur = st_gstate_ts_after - st_gstate_ts_before;
        std::chrono::duration<double, std::milli> mt_tstate_ms_dur = mt_tstate_ts_after - mt_tstate_ts_before;
        std::chrono::duration<double, std::milli> mt_gstate_ms_dur = mt_gstate_ts_after - mt_gstate_ts_before;

        fmt::println("st_tstate: {0}", st_tstate_ms_dur);
        fmt::println("st_gstate: {0}", st_gstate_ms_dur);
        fmt::println("mt_tstate: {0}", mt_tstate_ms_dur);
        fmt::println("mt_gstate: {0}", mt_gstate_ms_dur);
}