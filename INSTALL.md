# Installation guide

## Requirements
- CMake >= 3.10
- C++17 compatible compiler (GCC, Clang, MSVC)
- vcpkg (optional, for dependency management)
- Craw
- jwt-cpp

## Installation steps

### Installing vcpkg
See official installation guide [here](https://learn.microsoft.com/lt-lt/vcpkg/get_started/get-started-packaging?pivots=shell-powershell)

Do not forget to restart your terminal / IDE after installing vcpkg!

### Linking vcpkg to CMake
Place the following lines in your ./cmake/LocalConfig.cmake file:
```cmake
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(CMAKE_PREFIX_PATH "$ENV{VCPKG_ROOT}/installed/x64-windows" "$ENV{VCPKG_ROOT}/installed/x86-windows" ${CMAKE_PREFIX_PATH})
```

These paths may vary depending on your OS and architecture. For example, if you are using Linux, the path would be `$ENV{VCPKG_ROOT}/installed/x64-linux`.


### Installing dependencies using vcpkg
Run the following command in your terminal:
```bash
vcpkg install asio crow jwt-cpp
```

## Dependencies used in this project
- Crow [Source](https://github.com/CrowCpp/Crow)
- jwt-cpp [Source](https://github.com/Thalhammer/jwt-cpp)