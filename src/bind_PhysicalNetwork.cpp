//
// Created by thomas on 26.01.23.
//

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

