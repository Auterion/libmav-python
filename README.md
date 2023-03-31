# Libmav Python

Native python bindings for the [libmav](https://github.com/ThomasDebrunner/libmav) library.

## Installation

```
pip install libmav // Coming soon!
```

## Install from source

### Prerequisites

* A compiler with C++11 support
* Pip 10+ or CMake >= 3.4 (or 3.14+ on Windows, which was the first version to support VS 2019)
* Ninja or Pip 10+

Just clone this repository and pip install. Note the `--recursive` option which is
needed for the pybind11 submodule:

```bash
git clone --recursive https://github.com/ThomasDebrunner/cmake_example.git
pip install ./cmake_example
```

With the `setup.py` file included in this project, the `pip install` command will
invoke CMake and build the pybind11 module as specified in `CMakeLists.txt`.