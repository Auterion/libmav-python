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
#include "mav/UDPClient.h"
#include "mav/UDPServer.h"
#include "mav/TCPServer.h"
#include "mav/TCPClient.h"
#include "mav/Serial.h"

namespace py = pybind11;
using namespace mav;


void bind_PhysicalNetwork(py::module m) {
    py::class_<UDPClient, NetworkInterface>(m, "UDPClient")
            .def(py::init<const std::string&, int>(), py::arg("remote_address"), py::arg("remote_port"))
            .def("close", &UDPClient::close);

    py::class_<UDPServer, NetworkInterface>(m, "UDPServer")
            .def(py::init<int>(), py::arg("local_port"))
            .def("close", &UDPServer::close);

    py::class_<TCPServer, NetworkInterface>(m, "TCPServer")
            .def(py::init<int>(), py::arg("local_port"))
            .def("close", &TCPServer::close);

    py::class_<TCPClient, NetworkInterface>(m, "TCPClient")
            .def(py::init<const std::string&, int>(), py::arg("remote_address"), py::arg("remote_port"))
            .def("close", &TCPClient::close);

    py::class_<Serial, NetworkInterface>(m, "Serial")
            .def(py::init<const std::string&, int, bool>(), py::arg("device"),
                 py::arg("baudrate"), py::arg("flow_control"))
            .def("close", &Serial::close);
}

