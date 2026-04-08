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

# Link libatomic when available (required on some architectures like aarch64)
include(CheckCXXSourceCompiles)
set(CMAKE_REQUIRED_LIBRARIES atomic)
check_cxx_source_compiles("
    #include <atomic>
    int main() { std::atomic<bool> a{false}; return a.load(); }
" HAS_LIBATOMIC)
unset(CMAKE_REQUIRED_LIBRARIES)
if(HAS_LIBATOMIC)
    target_link_libraries(libmav PRIVATE atomic)
endif()

# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(libmav
                           PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
