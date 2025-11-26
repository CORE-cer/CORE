# CMake toolchain for ARM64 Linux with GCC 14 and libstdc++
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Compiler settings
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# C and C++ standards
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)  # gnu23

# GMP C17 workaround - force C17 for GMP and m4 dependencies
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu17")
