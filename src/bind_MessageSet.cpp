//
// Created by thomas on 26.01.23.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "mav/MessageSet.h"

namespace py = pybind11;
using namespace mav;


void bind_MessageSet(py::module m) {
    py::class_<MessageSet>(m, "MessageSet")
            .def(py::init<>())
            .def(py::init<const std::string&>())
            .def("create", static_cast<Message(MessageSet::*)(const std::string&) const>(&MessageSet::create))
            .def("create", static_cast<Message(MessageSet::*)(int) const>(&MessageSet::create))
            .def("idForMessage", &MessageSet::idForMessage)
            .def("addFromXMLString", &MessageSet::addFromXMLString)
            .def("addFromXMLFile", &MessageSet::addFromXML)
            .def("__len__", &MessageSet::size)
            .def("__contains__", static_cast<bool(MessageSet::*)(const std::string&) const>(&MessageSet::contains))
            .def("__contains__", static_cast<bool(MessageSet::*)(int) const>(&MessageSet::contains));
}


