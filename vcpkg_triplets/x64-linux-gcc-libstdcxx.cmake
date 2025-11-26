# Custom vcpkg triplet for x86_64 Linux with GCC and libstdc++
set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)

set(VCPKG_CMAKE_SYSTEM_NAME Linux)

# Chainload toolchain will be set via CMake variable
