//
// Created by thomas on 26.01.23.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "mav/MessageDefinition.h"

namespace py = pybind11;
using namespace mav;


void bind_MessageDefinition(py::module m) {

    py::class_<Identifier>(m, "Identifier")
            .def(py::init<uint8_t, uint8_t>());

    py::class_<Header<uint8_t*>>(m, "Header")
            .def_property("magic",
                 [](const Header<uint8_t*>& h) {return h.magic();},
                 [](Header<uint8_t*>& h, int value) {h.magic() = static_cast<uint8_t>(value);})
            .def_property("len",
                  [](const Header<uint8_t*>& h) {return h.len();},
                  [](Header<uint8_t*>& h, int value) {h.len() = static_cast<uint8_t>(value);})
            .def_property("incompat_flags",
                  [](const Header<uint8_t*>& h) {return h.incompatFlags();},
                  [](Header<uint8_t*>& h, int value) {h.incompatFlags() = static_cast<uint8_t>(value);})
            .def_property("compat_flags",
                  [](const Header<uint8_t*>& h) {return h.compatFlags();},
                  [](Header<uint8_t*>& h, int value) {h.compatFlags() = static_cast<uint8_t>(value);})
            .def_property("seq",
                  [](const Header<uint8_t*>& h) {return h.seq();},
                  [](Header<uint8_t*>& h, int value) {h.seq() = static_cast<uint8_t>(value);})
            .def_property("system_id",
                  [](const Header<uint8_t*>& h) {return h.systemId();},
                  [](Header<uint8_t*>& h, int value) {h.systemId() = static_cast<uint8_t>(value);})
            .def_property("component_id",
                  [](const Header<uint8_t*>& h) {return h.componentId();},
                  [](Header<uint8_t*>& h, int value) {h.componentId() = static_cast<uint8_t>(value);})
            .def_property("message_id",
                  [](const Header<uint8_t*>& h) {return static_cast<int>(h.msgId());},
                  [](Header<uint8_t*>& h, int value) {h.msgId() = value;});


    py::class_<MessageDefinition>(m, "MessageDefinition")
            .def_property_readonly("id", &MessageDefinition::id)
            .def_property_readonly("name", &MessageDefinition::name)
            .def_property_readonly("max_buffer_length", &MessageDefinition::maxBufferLength)
            .def_property_readonly("max_payload_size", &MessageDefinition::maxPayloadSize)
            .def_property_readonly("crc_extra", &MessageDefinition::crcExtra)
            .def("keys", &MessageDefinition::fieldNames)
            .def("field_names", &MessageDefinition::fieldNames)
            .def("__in__", [](MessageDefinition &m, const std::string &key) { return m.containsField(key);});
}