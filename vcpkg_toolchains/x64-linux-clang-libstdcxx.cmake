# CMake toolchain for x86_64 Linux with Clang 19 and libstdc++
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Compiler settings
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# C and C++ standards
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)  # gnu23

# Use libstdc++ instead of libc++
set(CMAKE_CXX_FLAGS_INIT "-stdlib=libstdc++")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-stdlib=libstdc++")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-stdlib=libstdc++")

# Architecture flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")

# GMP C17 workaround - force C17 for GMP and m4 dependencies
# This is handled at the package level in vcpkg, but we keep the flag available
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu17")
