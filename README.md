<a href="https://github.com/moddyz/CXXSandbox/actions?query=workflow%3A%22Build+and+test%22"><img src="https://github.com/moddyz/CXXSandbox/workflows/Build%20and%20test/badge.svg"/></a>

# CXXSandbox

Sandbox for experimenting with the C++ language.

## Table of Contents

- [Dependencies](#dependencies)
- [Building](#building)

### Dependencies

The following dependencies are mandatory:
- C++ compiler
- [CMake](https://cmake.org/documentation/) (3.12 or greater)
- [TBB](https://github.com/oneapi-src/oneTBB)

## Building

Example snippet for building this project:
```
mkdir build && cd build
cmake -DBUILD_TESTING=ON -DCMAKE_INSTALL_PREFIX="/apps/CXXSandbox/" ..
cmake --build  . -- VERBOSE=1 -j8 all test install
```
CMake options for configuring this project:

| CMake Variable name     | Description                                                            | Default |
| ----------------------- | ---------------------------------------------------------------------- | ------- |
| `BUILD_TESTING`         | Enable automated testing.                                              | `OFF`   |
