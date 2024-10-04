#include <boost/python.hpp>

#include <string>
#include <vector>
#include <thread>

// runs in a new thread
void f(PyInterpreterState* interp, const char* tname)
{
    std::string code = R"PY(

import sys

print("TNAME: sys.xxx={}".format(getattr(sys, 'xxx', 'attribute not set')))

    )PY";

    code.replace(code.find("TNAME"), 5, tname);

    PyThreadState* ts = PyThreadState_New(interp);
    PyEval_RestoreThread(ts);
    PyThreadState* _ts = PyThreadState_Swap(ts);

    //boost::python::str s{"core"};
    PyUnicode_FromString("core");
    PyRun_SimpleString(code.c_str());

    PyThreadState_Swap(_ts);
    PyThreadState_Clear(ts);
    PyThreadState_DeleteCurrent();
}

int main()
{
    Py_InitializeEx(1);

    PyThreadState* ts_main = PyThreadState_Get();

    PyThreadState* ts_interp1 = Py_NewInterpreter();
    PyThreadState_Swap(ts_main);

    PyThreadState* ts_interp2 = Py_NewInterpreter();
    PyThreadState_Swap(ts_main);

    PyRun_SimpleString(R"PY(

# set sys.xxx, it will only be reflected in t4, which runs in the context of the main interpreter

import sys

sys.xxx = ['abc']
print('main: setting sys.xxx={}'.format(sys.xxx))

    )PY");

    std::thread t1{f, ts_interp1->interp, "t1(s1)"};
    std::thread t2{f, ts_interp2->interp, "t2(s2)"};
    std::thread t3{f, ts_interp1->interp, "t3(s1)"};
    std::thread t4{f, ts_main->interp, "t4(main)"};

    PyThreadState* _state = PyEval_SaveThread();

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    PyEval_RestoreThread(_state);

    PyThreadState_Swap(ts_interp1);
    Py_EndInterpreter(ts_interp1);

    PyThreadState_Swap(ts_interp2);
    Py_EndInterpreter(ts_interp2);

    return 0;
}