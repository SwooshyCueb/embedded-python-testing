#include <boost/python.hpp>

#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

void f(PyInterpreterState* interp)
{
        PyThreadState* ts = PyThreadState_New(interp);
        PyEval_RestoreThread(ts);
        PyThreadState* _ts = PyThreadState_Swap(ts);

        {
                boost::python::str s{"core"};
        }

        PyThreadState_Swap(_ts);
        PyThreadState_Clear(ts);
        PyThreadState_DeleteCurrent();
}

constexpr uint THREADS_NUM = 4;

int main()
{
        Py_InitializeEx(0);

        std::vector<std::thread> v;
        v.reserve(THREADS_NUM);
        
        PyThreadState* ts_main = PyEval_SaveThread();

        f(ts_main->interp);
        f(ts_main->interp);
        for (int i = 0; i < THREADS_NUM; ++i) {
                v.emplace_back(f, ts_main->interp);
        }
        for (int i = 0; i < THREADS_NUM; ++i)
                v[i].join();
        f(ts_main->interp);
        f(ts_main->interp);
        
        PyEval_RestoreThread(ts_main);
}