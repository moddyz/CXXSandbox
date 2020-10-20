![Build and test](https://github.com/moddyz/CXXSandbox/workflows/Build%20and%20test/badge.svg)

# CXXSandbox

Various experiments with C++.

## Table of Contents

- [Dependencies](#dependencies)
- [Building](#building)

### Dependencies

The following dependencies are mandatory:
- C++ compiler
- [CMake](https://cmake.org/documentation/) (3.12 or greater)

The following dependencies are optional:
- [Doxygen](https://www.doxygen.nl/index.html) and [graphiviz](https://graphviz.org/) for documentation.

## Building

Example snippet for building this project:
```
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX="/apps/CXXSandbox/" ..
cmake --build  . -- VERBOSE=1 -j8 all test install
```
CMake options for configuring this project:

| CMake Variable name     | Description                                                            | Default |
| ----------------------- | ---------------------------------------------------------------------- | ------- |
| `BUILD_TESTING`         | Enable automated testing.                                              | `OFF`   |
| `BUILD_DOCUMENTATION`   | Build documentation.                                                   | `OFF`   |
