// #include <Python.h>
#include <iostream>
#include <python3.10/Python.h>
int main()
{
    // 初始化Python解释器
    Py_Initialize();

    // 导入Python模块和变量
    PyObject* module = PyImport_ImportModule("your_script");
    if (!module) {
        std::cerr << "Failed to import module" << std::endl;
        return 1;
    }

    PyObject* variable = PyObject_GetAttrString(module, "result");
    if (!variable) {
        std::cerr << "Failed to get variable" << std::endl;
        return 1;
    }

    // 将Python变量转换为C++类型
    double result;
    PyArg_Parse(variable, "d", &result);

    // 打印结果
    std::cout << "Result: " << result << std::endl;

    // 释放Python对象和解释器
    Py_XDECREF(variable);
    Py_XDECREF(module);
    Py_Finalize();

    return 0;
}
