#include <boost/python.hpp>

#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

namespace
{
        namespace python_state
        {
                static PyThreadState* ts_main;

                static thread_local PyThreadState* ts_thread;
                static thread_local PyThreadState* ts_thread_old;
                static thread_local uint64_t ts_thread_refct = 0;
        }

        struct python_tstate_scope
        {
                python_tstate_scope(PyInterpreterState* interp)
                {
                        if (python_state::ts_thread_refct == 0) {
                                python_state::ts_thread = PyThreadState_New(interp);
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
}

void f(PyInterpreterState* interp)
{
        python_tstate_scope tstate(interp);

        {
                python_tstate_scope tstate2(interp);
                {
                        boost::python::str s{"core"};
                }
        }
}

constexpr uint THREADS_NUM = 4;

int main()
{
        Py_InitializeEx(0);

        std::vector<std::thread> v;
        v.reserve(THREADS_NUM);
        
        python_state::ts_main = PyEval_SaveThread();

        f(python_state::ts_main->interp);
        f(python_state::ts_main->interp);
        for (int i = 0; i < THREADS_NUM; ++i) {
                v.emplace_back(f, python_state::ts_main->interp);
        }
        for (int i = 0; i < THREADS_NUM; ++i)
                v[i].join();
        f(python_state::ts_main->interp);
        f(python_state::ts_main->interp);
        
        PyEval_RestoreThread(python_state::ts_main);
}