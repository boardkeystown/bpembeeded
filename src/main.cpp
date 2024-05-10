#define BOOST_NO_AUTO_PTR

/*
boost/python/converter/pyobject_tyect_type.hpp:13:11: 
warning: inline function 'PyObject* boost::python::conv:checkerter::checked_downcast_impl(PyObject*, PyTypeObject*)' 
declared as dllimpoe ignort: attribute ignored [-Wattributes]
   13 | PyObject* checked_downcast_impl(PyObject *obj, PyTypeObject *type)       
      |           ^~~~~~~~~~~~~~~~~~~~~
*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <boost/python.hpp>
#pragma GCC diagnostic pop
/*
//And for MSVC:
//cpp
//Copy code
#pragma warning(push)
#pragma warning(disable : 4251)  // ??? 
#include <boost/python.hpp>
#pragma warning(pop)
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

struct FooBar {
    int a;
    float f;
    std::string str;
    FooBar() : 
        a(0),
        f(0.f),
        str("")
    {}
    ~FooBar() {}
};

std::string readFileToString(const std::string& filePath) {
    std::ifstream fileStream(filePath);
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}

int counter_val = 0;

void counter() {
    std::cout << ++counter_val << "\n";
}

BOOST_PYTHON_MODULE(my_cpp_module) { // Define module name here
    using namespace boost::python;
    def("counter", &counter); // Expose the counter function
    class_<FooBar>("FooBar", init<>())
        .def_readwrite("a",&FooBar::a)
        .def_readwrite("f",&FooBar::f)
        .def_readwrite("str",&FooBar::str)
    ;
}

// Function to initialize the embedded module
void init_module() {
    if (PyImport_AppendInittab("my_cpp_module", &PyInit_my_cpp_module) == -1) {
        throw std::runtime_error(
            "Failed to append 'my_cpp_module' to the built-in modules table"
        );
    }
}
int main() {
    bool weGood = false;
    boost::python::object entrypoint;
    boost::python::object passobject;
    FooBar fooBar;
    try {
        init_module(); // Initialize the embedded module before any Python code runs
        Py_Initialize();
        // Import the __main__ module
        boost::python::object main_module = boost::python::import("__main__");
        boost::python::object main_namespace = main_module.attr("__dict__");
        // Load your Python script
        boost::python::exec(
            readFileToString("your_python_script.py").c_str(),
            main_namespace,
            main_namespace
        );
        entrypoint = boost::python::dict(main_namespace).get("entrypoint");
        passobject = boost::python::dict(main_namespace).get("passobject");
        weGood = true;
    } catch (const boost::python::error_already_set&) {
        PyErr_Print();
    } catch (const std::exception &e){
        std::cout << e.what() << "\n";
    }
    if (weGood) {
        if (PyCallable_Check(entrypoint.ptr())) {
            entrypoint(); 
            entrypoint(); 
            entrypoint(); 
            entrypoint(); 
        } else {
            std::cerr << "Error: 'entrypoint' is not callable or not defined.\n";
        }
        if (PyCallable_Check(passobject.ptr())) {
            passobject(boost::ref(fooBar));
            std::cout 
            << "after python "
            << " fooBar.a=" 
            << fooBar.a 
            << " fooBar.f=" 
            << fooBar.f
            << " fooBar.str=" 
            << "'"
            << fooBar.str
            << "'"
            << "\n";
        } else {
            std::cerr << "Error: 'passobject' is not callable or not defined.\n";
        }
    }
    // Don't call Py_Finalize with Boost.Python
    return 0;
}