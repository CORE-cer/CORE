# Custom vcpkg triplet for x86_64 Linux with Clang and libstdc++
set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)

set(VCPKG_CMAKE_SYSTEM_NAME Linux)

# Use clang/clang++ for CMake-based ports
set(VCPKG_CMAKE_CONFIGURE_OPTIONS -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++)
set(VCPKG_CXX_FLAGS "-stdlib=libstdc++")
set(VCPKG_C_FLAGS "-stdlib=libstdc++")
set(VCPKG_LINKER_FLAGS "-stdlib=libstdc++")
