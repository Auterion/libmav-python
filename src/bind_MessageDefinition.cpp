/****************************************************************************
 * 
 * Copyright (c) 2023, libmav development team
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in 
 *    the documentation and/or other materials provided with the 
 *    distribution.
 * 3. Neither the name libmav nor the names of its contributors may be 
 *    used to endorse or promote products derived from this software 
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 ****************************************************************************/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "mav/MessageDefinition.h"

namespace py = pybind11;
using namespace mav;


void bind_MessageDefinition(py::module m) {

    py::class_<ConnectionPartner>(m, "ConnectionPartner")
            .def_property_readonly("address", &ConnectionPartner::address)
            .def_property_readonly("port", &ConnectionPartner::port)
            .def_property_readonly("is_uart", &ConnectionPartner::isUart)
            .def("__repr__", &ConnectionPartner::toString);

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