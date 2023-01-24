#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "libmavlink/utils.h"
#include "libmavlink/MessageSet.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

int add(int i, int j) {
    return i + j;
}

namespace py = pybind11;
using namespace mav;

PYBIND11_MODULE(libmav, m) {
    m.doc() = R"pbdoc(
        libmav python plugin
        -----------------------

        .. currentmodule:: cmake_example

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";

    py::class_<CRC>(m, "CRC")
            .def(py::init<>())
            .def("accumulate", static_cast<void (CRC::*)(const std::string_view&)>(&CRC::accumulate), "Bla")
            .def("crc16", &CRC::crc16)
            .def("crc8", &CRC::crc8);



    py::class_<MessageSet>(m, "MessageSet")
            .def(py::init<>())
            .def(py::init<const std::string&>())
            .def("createMessage", &MessageSet::createMessage)
            .def("idForMessage", &MessageSet::idForMessage)
            .def("addFromXMLString", &MessageSet::addFromXMLString)
            .def("addFromXMLFile", &MessageSet::addFromXML);

    py::class_<Message>(m, "Message")
            .def("id", &Message::id)
            .def("name", &Message::name)
            .def("header", &Message::header)
            .def("__getitem__", &Message::getAsNativeTypeInVariant)
            .def("__setitem__", &Message::set<int64_t>,
                    py::arg("field_key"), py::arg("value"), py::arg("array_index") = 0)
            .def("__setitem__", &Message::set<uint64_t>,
                    py::arg("field_key"), py::arg("value"), py::arg("array_index") = 0)
            .def("__setitem__", &Message::set<float>,
                    py::arg("field_key"), py::arg("value"), py::arg("array_index") = 0)
            .def("__setitem__", &Message::set<double>,
                    py::arg("field_key"), py::arg("value"), py::arg("array_index") = 0)
            .def("__setitem__", &Message::set<const std::vector<int64_t>&>,
                    py::arg("field_key"), py::arg("value"), py::arg("array_index") = 0)
            .def("__setitem__", &Message::set<const std::vector<uint64_t>&>,
                    py::arg("field_key"), py::arg("value"), py::arg("array_index") = 0)
            .def("__setitem__", &Message::set<const std::vector<float>&>,
                    py::arg("field_key"), py::arg("value"), py::arg("array_index") = 0)
            .def("__setitem__", &Message::set<const std::vector<double>&>,
                    py::arg("field_key"), py::arg("value"), py::arg("array_index") = 0)
            .def("__setitem__", &Message::set<const std::string&>,
                    py::arg("field_key"), py::arg("value"), py::arg("array_index") = 0);

    m.def("add", &add, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
