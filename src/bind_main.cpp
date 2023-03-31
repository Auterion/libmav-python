#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

void bind_Message(py::module);
void bind_utils(py::module);
void bind_MessageSet(py::module);
void bind_MessageDefinition(py::module);
void bind_NetworkRuntime(py::module);
void bind_Connection(py::module);
void bind_PhysicalNetwork(py::module);


PYBIND11_MODULE(libmav, m) {
    m.doc() = R"pbdoc(
        libmav python plugin
        -----------------------
        .. currentmodule:: libmav

        .. autosummary::
           :toctree: _generate

    )pbdoc";

    bind_Message(m);
    bind_utils(m);
    bind_MessageSet(m);
    bind_MessageDefinition(m);
    bind_NetworkRuntime(m);
    bind_Connection(m);
    bind_PhysicalNetwork(m);


#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
