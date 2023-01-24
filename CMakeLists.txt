cmake_minimum_required(VERSION 3.4...3.18)
project(libmav)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(pybind11)
pybind11_add_module(libmav src/main.cpp)

# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(libmav
                           PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
