//
// Created by thomas on 26.01.23.
//

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include "mav/Message.h"
#include "mav/MessageFieldIterator.h"

namespace py = pybind11;
using namespace mav;



void setFromDict(Message &m, const py::dict &d) {
    for (auto item : d) {
        auto key = item.first.cast<std::string>();
        // skip underline prefixed fields
        if (!key.empty() && key[0] != '_') {
            m.setFromNativeTypeVariant(
                    key, item.second.cast<NativeVariantType>());
        }
    }
}

py::dict toDict(const Message &m) {
    py::dict d;
    d["_id"] = m.id();
    d["_name"] = m.name();
    for (auto item : mav::FieldIterate(m)) {
        d[py::str(item.first)] = item.second;
    }
    return d;
}


void bind_Message(py::module m) {
    py::class_<Message>(m, "Message")
            .def_property_readonly("id", &Message::id)
            .def_property_readonly("name", &Message::name)
            .def_property_readonly("type", &Message::type)
            .def_property_readonly("header", static_cast<Header<uint8_t*>(Message::*)(void)>(&Message::header))
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
                 py::arg("field_key"), py::arg("value"), py::arg("array_index") = 0)
            .def("set_from_dict", [](Message &m, const py::dict &d) {
                setFromDict(m, d);
                return m;
            })
            .def("to_dict", [](const Message &m) {
                return toDict(m);
            })
            .def("__iter__", [](const Message &m) { return py::make_iterator(
                    mav::FieldIterate(m).begin(), mav::FieldIterate(m).end()); }, py::keep_alive<0, 1>())
            .def("__contains__", [](Message &m, const std::string &key) { return m.type().containsField(key);})
            .def("__repr__", &Message::toString);
}
