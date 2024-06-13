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
#include "mav/Connection.h"
#include <queue>
#include <mutex>
#include <optional>

namespace py = pybind11;
using namespace mav;

struct _ExpectationWrapper {
    Connection::Expectation expectation;
};

// Class to queue incoming messages from a Connection to be accessed asynchronously by python
class MessageQueue {
private:
    std::queue<Message> _messages;
    std::mutex _lock;
    std::weak_ptr<Connection> _connection;
    CallbackHandle _cb_handle;
public:
    MessageQueue(std::shared_ptr<Connection> &connection) : _connection(connection) {
        _cb_handle = connection->addMessageCallback([this](const Message &message) {
            std::lock_guard lg{_lock};
            _messages.push(message);
        });
    }

    ~MessageQueue() {
        auto connection = _connection.lock();
        if (connection) {
            connection->removeMessageCallback(_cb_handle);
        }
    }

    std::optional<Message> next() {
        std::lock_guard lg{_lock};
        if (_messages.empty()) {
            return std::nullopt;
        }
        const Message ret = _messages.front();
        _messages.pop();
        return ret;
    }

    std::size_t size() {
        std::lock_guard lg{_lock};
        return _messages.size();
    }
};


void bind_Connection(py::module m) {
    py::class_<_ExpectationWrapper>(m, "_ExpectationWrapper")
            .def(py::init<>());

    py::class_<MessageQueue>(m, "MessageQueue")
            .def(py::init<std::shared_ptr<Connection> &>())
            .def("next", &MessageQueue::next, py::call_guard<py::gil_scoped_release>())
            .def("__iter__", [](MessageQueue &self) -> MessageQueue & { return self; })
            .def("__next__", [](MessageQueue &self) {
                py::gil_scoped_release release;
                auto msg = self.next();
                if (!msg) {
                    throw py::stop_iteration();
                }
                return *msg;
            })
            .def("__len__", &MessageQueue::size, py::call_guard<py::gil_scoped_release>());

    py::class_<Connection, std::shared_ptr<Connection>>(m, "Connection")
            .def("alive", &Connection::alive)
            .def("partner", &Connection::partner)
            .def("send", &Connection::send, py::call_guard<py::gil_scoped_release>())
            .def("add_message_callback",
                 (&Connection::addMessageCallback<std::function<void(const Message &)>>),
                 py::call_guard<py::gil_scoped_release>())
            .def("add_message_callback",
                 (&Connection::addMessageCallback<std::function<void(const Message &)>,
                         std::function<void(const std::exception_ptr &)>>),
                 py::call_guard<py::gil_scoped_release>())
            .def("remove_message_callback", &Connection::removeMessageCallback,
                 py::call_guard<py::gil_scoped_release>())
            .def("expect",
                 [](Connection &self, int message_id, int source_id, int component_id) {
                     py::gil_scoped_release release;
                     return _ExpectationWrapper{self.expect(message_id, source_id, component_id)};
                 }, py::arg("message_id"), py::arg("source_id") = mav::ANY_ID,
                 py::arg("component_id") = mav::ANY_ID)
            .def("expect", [](Connection &self, const std::string &message_name, int source_id, int component_id) {
                     py::gil_scoped_release release;
                     return _ExpectationWrapper{self.expect(message_name, source_id, component_id)};
                 }, py::arg("message_name"), py::arg("source_id") = mav::ANY_ID,
                 py::arg("component_id") = mav::ANY_ID)
            .def("receive",
                 [](Connection &self, const _ExpectationWrapper &expectation, int timeout_ms) {
                     py::gil_scoped_release release;
                     return self.receive(expectation.expectation, timeout_ms);
                 }, py::arg("expectation"), py::arg("timeout_ms") = -1)
            .def("receive",
                 py::overload_cast<const std::string &, int, int, int>(&Connection::receive),
                 py::call_guard<py::gil_scoped_release>(),
                 py::arg("message_name"), py::arg("source_id"),
                 py::arg("component_id"), py::arg("timeout_ms") = -1)
            .def("receive", py::overload_cast<const std::string &, int>(&Connection::receive),
                 py::call_guard<py::gil_scoped_release>(),
                 py::arg("message_name"), py::arg("timeout_ms") = -1)
            .def("receive", py::overload_cast<int, int, int, int>(&Connection::receive),
                 py::call_guard<py::gil_scoped_release>(),
                 py::arg("message_id"), py::arg("source_id") = mav::ANY_ID,
                 py::arg("component_id") = mav::ANY_ID, py::arg("timeout_ms") = -1)
            .def("receive", py::overload_cast<int, int>(&Connection::receive),
                 py::call_guard<py::gil_scoped_release>(),
                 py::arg("message_id"), py::arg("timeout_ms") = -1);
}
