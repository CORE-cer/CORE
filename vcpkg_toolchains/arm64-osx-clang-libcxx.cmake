# CMake toolchain for ARM64 macOS with Clang 19 and libc++
set(CMAKE_SYSTEM_NAME Darwin)
set(CMAKE_SYSTEM_PROCESSOR arm64)
set(CMAKE_OSX_ARCHITECTURES arm64)

# Compiler settings
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# C and C++ standards
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)  # gnu23

# Use libc++ (default on macOS)
set(CMAKE_CXX_FLAGS_INIT "-stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-stdlib=libc++")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-stdlib=libc++")

# GMP C17 workaround - force C17 for GMP and m4 dependencies
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu17")
