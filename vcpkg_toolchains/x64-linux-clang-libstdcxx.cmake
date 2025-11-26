# CMake toolchain for x86_64 Linux with Clang 19 and libstdc++
# This toolchain is chainloaded by vcpkg and applies to the main project only

# Only set compiler if not building vcpkg port
if(NOT VCPKG_CHAINLOAD_TOOLCHAIN_FILE OR "${PROJECT_NAME}" STREQUAL "core")
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
endif()
