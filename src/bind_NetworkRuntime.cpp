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
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "mav/Network.h"

namespace py = pybind11;
using namespace mav;


void bind_NetworkRuntime(py::module m) {
    py::class_<NetworkInterface>(m, "NetworkInterface");

    py::class_<NetworkRuntime>(m, "NetworkRuntime")
            .def(py::init<const Identifier&, const MessageSet&, NetworkInterface&>(), py::keep_alive<1, 4>(),
                    py::arg("own_mavlink_id"), py::arg("message_set"), py::arg("interface"))
            .def(py::init<const MessageSet&, NetworkInterface&>(), py::keep_alive<1, 3>(),
                    py::arg("message_set"), py::arg("interface"))
            .def(py::init<const Identifier&, const MessageSet&, const Message&, NetworkInterface&>(), py::keep_alive<1, 5>(),
                    py::arg("own_mavlink_id"), py::arg("message_set"), py::arg("heartbeat_message"), py::arg("interface"))
            .def(py::init<const MessageSet&, const Message&, NetworkInterface&>(), py::arg("message_set"), py::keep_alive<1, 4>(),
                    py::arg("heartbeat_message"), py::arg("interface"))
            .def("on_connection", &NetworkRuntime::onConnection)
            .def("on_connection_lost", &NetworkRuntime::onConnectionLost)
            .def("await_connection", &NetworkRuntime::awaitConnection)
            .def("set_heartbeat_message", &NetworkRuntime::setHeartbeatMessage)
            .def("clear_heartbeat_message", &NetworkRuntime::clearHeartbeat);
}

