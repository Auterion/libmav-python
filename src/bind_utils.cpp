//
// Created by thomas on 26.01.23.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "mav/utils.h"

namespace py = pybind11;
using namespace mav;


void bind_utils(py::module m) {
    py::class_<CRC>(m, "CRC")
            .def(py::init<>())
            .def("accumulate", static_cast<void (CRC::*)(const std::string_view&)>(&CRC::accumulate), "Bla")
            .def("crc16", &CRC::crc16)
            .def("crc8", &CRC::crc8);

}