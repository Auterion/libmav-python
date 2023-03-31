cmake_minimum_required(VERSION 3.4...3.18)
project(libmav)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(pybind11)
pybind11_add_module(libmav
        src/bind_main.cpp
        src/bind_Message.cpp
        src/bind_utils.cpp
        src/bind_MessageSet.cpp
        src/bind_MessageDefinition.cpp
        src/bind_NetworkRuntime.cpp
        src/bind_Connection.cpp
        src/bind_PhysicalNetwork.cpp)

target_include_directories(libmav PRIVATE src/libmav/include)

# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(libmav
                           PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
