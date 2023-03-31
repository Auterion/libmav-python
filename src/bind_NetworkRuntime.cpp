//
// Created by thomas on 26.01.23.
//

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

