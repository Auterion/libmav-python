//
// Created by thomas on 26.01.23.
//

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "mav/Connection.h"

namespace py = pybind11;
using namespace mav;

struct _ExpectationWrapper {
    Connection::Expectation expectation;
};


void bind_Connection(py::module m) {
    py::class_<_ExpectationWrapper>(m, "_ExpectationWrapper")
            .def(py::init<>());

    py::class_<Connection, std::shared_ptr<Connection>>(m, "Connection")
            .def("alive", &Connection::alive)
            .def("partner", &Connection::partner)
            .def("send", &Connection::send)
            .def("add_message_callback",
                (&Connection::addMessageCallback<std::function<void(const Message&)>>))
            .def("add_message_callback",
                (&Connection::addMessageCallback<std::function<void(const Message&)>,
                        std::function<void(const std::exception_ptr&)>>))
            .def("remove_message_callback", &Connection::removeMessageCallback)
            .def("expect",
                 [](Connection& self, int message_id, int source_id, int component_id) {
                     return _ExpectationWrapper {self.expect(message_id, source_id, component_id)};
                 }, py::arg("message_id"), py::arg("source_id") = mav::ANY_ID,
                    py::arg("component_id") = mav::ANY_ID)
            .def("expect", [](Connection& self, const std::string& message_name, int source_id, int component_id) {
                return _ExpectationWrapper {self.expect(message_name, source_id, component_id)};
            }, py::arg("message_name"), py::arg("source_id") = mav::ANY_ID,
                    py::arg("component_id") = mav::ANY_ID)
            .def("receive",
                 [](Connection& self, const _ExpectationWrapper& expectation, int timeout_ms) {
                     return self.receive(expectation.expectation, timeout_ms);
                 }, py::arg("expectation"), py::arg("timeout_ms") = -1)
            .def("receive",
                 py::overload_cast<const std::string&, int, int, int>(&Connection::receive),
                    py::arg("message_name"), py::arg("source_id") = mav::ANY_ID,
                    py::arg("component_id") = mav::ANY_ID, py::arg("timeout_ms") = -1)
            .def("receive", py::overload_cast<const std::string&, int>(&Connection::receive),
                    py::arg("message_name"), py::arg("timeout_ms") = -1)
            .def("receive", py::overload_cast<int, int, int, int>(&Connection::receive),
                    py::arg("message_id"), py::arg("source_id") = mav::ANY_ID,
                    py::arg("component_id") = mav::ANY_ID, py::arg("timeout_ms") = -1)
            .def("receive", py::overload_cast<int, int>(&Connection::receive),
                    py::arg("message_id"), py::arg("timeout_ms") = -1);
}

