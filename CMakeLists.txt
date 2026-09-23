# Auto-detect vcpkg toolchain
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "Vcpkg toolchain file")
endif()

set(VCPKG_OVERLAY_TRIPLETS "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-triplets"
    CACHE STRING "Vcpkg overlay triplets directory")

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_COMPILER_LAUNCHER "ccache")

include(FetchContent)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# --- Optional experimental optimizations -------------------------------------
# The minterm-tree predicate evaluation (src/core_server/internal/optimizations/
# minterm_tree/) needs the Z3 SMT solver, which vcpkg builds from source (slow
# and large). It is therefore opt-in: with this option OFF nothing about the
# build changes, and Z3 is not even downloaded.
# This has to be decided BEFORE project(), because the vcpkg toolchain reads
# VCPKG_MANIFEST_FEATURES when project() runs.
option(CORE_ENABLE_MINTERM_OPTIMIZATION
       "Build the optional minterm-tree predicate evaluation (requires Z3)" OFF)
if(CORE_ENABLE_MINTERM_OPTIMIZATION)
  list(APPEND VCPKG_MANIFEST_FEATURES "minterm-optimization")
endif()

project (core CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src/)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(antlr4-runtime REQUIRED)
find_package(cereal REQUIRED)
find_package(cppzmq REQUIRED)
find_package(Boost REQUIRED)
find_package(re2 REQUIRED)
find_package(quill REQUIRED)
find_package(readerwriterqueue REQUIRED)
find_package(argparse REQUIRED)
if(CORE_ENABLE_MINTERM_OPTIMIZATION)
  find_package(Z3 REQUIRED)
endif()
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
   set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

# Profiler
message("Fetching Tracy")

# Output what compiler we are using
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")

Set(FETCHCONTENT_QUIET FALSE)

# Tracy must be configured BEFORE FetchContent_MakeAvailable so that
# its CMakeLists.txt sees the correct TRACY_ENABLE value.
if (PROFILING)
  set(TRACY_ENABLE ON CACHE BOOL "Enable Tracy" FORCE)
  set(TRACY_NO_EXIT ON CACHE BOOL "No Exit Tracy" FORCE)
  set(TRACY_DEBUGINFOD ON CACHE BOOL "Use DEBUGINFOD Tracy" FORCE)
else()
  set(TRACY_ENABLE OFF CACHE BOOL "Disable Tracy" FORCE)
endif()

FetchContent_Declare (
  tracy
  GIT_REPOSITORY https://github.com/wolfpld/tracy.git
  GIT_TAG c556831ddc6fe26d2fce01c14c97205a9dad46d5
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable (tracy)

if(TARGET TracyClient)
    set_target_properties(TracyClient PROPERTIES POSITION_INDEPENDENT_CODE ON)
    message(STATUS "Enabled POSITION_INDEPENDENT_CODE for TracyClient target.")
else()
    message(WARNING "TracyClient target was not found after FetchContent_MakeAvailable(tracy). The -fPIC fix might not be applied correctly.")
endif()

# Target in Makefile for generation of grammar from .g4 file.
add_custom_target(
  grammar
  COMMAND ${CMAKE_SOURCE_DIR}/src/core_server/internal/parsing/ceql_query/autogenerate_script.sh
  COMMAND ${CMAKE_SOURCE_DIR}/src/core_server/internal/parsing/stream_declaration/autogenerate_script.sh
  COMMAND ${CMAKE_SOURCE_DIR}/src/core_server/internal/parsing/option_declaration/autogenerate_script.sh
)

file(GLOB_RECURSE SHARED_FILES
     src/shared/*.cpp
)

file(GLOB_RECURSE CLIENT_FILES
     src/core_client/*.cpp
)

file(GLOB_RECURSE SERVER_FILES
     src/core_server/*.cpp
)

file(GLOB_RECURSE STREAMER_FILES
     src/core_streamer/*.cpp
)

file(GLOB_RECURSE PYBIND_FILES
     src/pybind/*.cpp
)

# Python/nanobind - FetchContent for dev builds (LSP/clang-tidy),
# pip-installed nanobind for wheel builds.
find_package(Python COMPONENTS Interpreter Development.Module QUIET)
if(Python_FOUND)
    find_package(nanobind CONFIG QUIET)
    if(NOT nanobind_FOUND)
        FetchContent_Declare(
          nanobind
          GIT_REPOSITORY https://github.com/wjakob/nanobind.git
          GIT_TAG        v2.4.0
          GIT_SHALLOW    TRUE
        )
        FetchContent_MakeAvailable(nanobind)
    endif()
endif()

add_library(core STATIC
            ${SHARED_FILES}
            ${CLIENT_FILES}
            ${SERVER_FILES}
            ${STREAMER_FILES})

set_target_properties(core PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(core PUBLIC antlr4_static cereal::cereal cppzmq Boost::boost Threads::Threads re2::re2 quill::quill TracyClient readerwriterqueue::readerwriterqueue argparse::argparse)
if (PROFILING)
  message("Profiling enabled")
  target_link_libraries(core PUBLIC debuginfod)
endif()

if(CORE_ENABLE_MINTERM_OPTIMIZATION)
  message("Minterm-tree optimization enabled (linking Z3)")
  # Only the factory translation unit gets the macro, so flipping the option
  # recompiles that one file instead of the whole engine. See the .cpp for why
  # the macro must never be visible in headers.
  set_source_files_properties(
    src/core_server/internal/optimizations/optimized_predicate_evaluator_factory.cpp
    PROPERTIES COMPILE_DEFINITIONS CORE_ENABLE_MINTERM_OPTIMIZATION)
  target_link_libraries(core PUBLIC z3::libz3)
endif()

# Set debug flag for core
if(CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_definitions(core PUBLIC CORE_DEBUG)
endif()

# Add compiler and linker flags to core
target_compile_options(core PUBLIC -Wno-unused-variable -Wno-unused-command-line-argument)
target_compile_options(core PRIVATE -Werror=old-style-cast)

if (ADDRESS_SANITIZER)
  message("Address sanitizer enabled")
  target_compile_options(core PUBLIC -fsanitize=address -fno-omit-frame-pointer -fsanitize-address-use-after-scope -fsanitize=undefined -fsanitize=leak)
  target_link_options(core PUBLIC -fsanitize=address -fno-omit-frame-pointer -fsanitize-address-use-after-scope -fsanitize=undefined -fsanitize=leak)
endif()

if (THREAD_SANITIZER)
  message("Thread sanitizer enabled")
  target_compile_options(core PUBLIC -fsanitize=thread -fno-omit-frame-pointer)
  target_link_options(core PUBLIC -fsanitize=thread -fno-omit-frame-pointer)
endif()

message("Logging level: ${LOGGING}")

if (LOGGING MATCHES "critical")
  target_compile_options(core PUBLIC -DQUILL_COMPILE_ACTIVE_LOG_LEVEL=QUILL_COMPILE_ACTIVE_LOG_LEVEL_CRITICAL)
endif()

if (LOGGING MATCHES "info")
  target_compile_options(core PUBLIC -DQUILL_COMPILE_ACTIVE_LOG_LEVEL=QUILL_COMPILE_ACTIVE_LOG_LEVEL_INFO)
endif()

if (LOGGING MATCHES "debug")
  target_compile_options(core PUBLIC -DQUILL_COMPILE_ACTIVE_LOG_LEVEL=QUILL_COMPILE_ACTIVE_LOG_LEVEL_DEBUG)
endif()

if (LOGGING MATCHES "trace_l1")
  target_compile_options(core PUBLIC -DQUILL_COMPILE_ACTIVE_LOG_LEVEL=QUILL_COMPILE_ACTIVE_LOG_LEVEL_TRACE_L1)
endif()

if (LOGGING MATCHES "trace_l2")
  target_compile_options(core PUBLIC -DQUILL_COMPILE_ACTIVE_LOG_LEVEL=QUILL_COMPILE_ACTIVE_LOG_LEVEL_TRACE_L2)
endif()

if (LOGGING MATCHES "trace_l3")
  target_compile_options(core PUBLIC -DQUILL_COMPILE_ACTIVE_LOG_LEVEL=QUILL_COMPILE_ACTIVE_LOG_LEVEL_TRACE_L3)
endif()

# Python bindings (must come after core target definition)
if(Python_FOUND)
    nanobind_add_module(pycer MODULE src/pybind/python_binding.cpp)
    target_compile_options(pycer PRIVATE -Wno-unused-variable -Wno-old-style-cast)
    target_link_libraries(pycer PRIVATE core)

    if(SKBUILD)
        # pip wheel build - install module and generate stubs
        install(TARGETS pycer DESTINATION .)
        nanobind_add_stub(
          pycer_stub
          INSTALL_TIME
          MODULE pycer
          OUTPUT pycer.pyi
          PYTHON_PATH "."
          MARKER_FILE py.typed
        )
    else()
        # Dev build - keep in compile_commands.json for LSP/clang-tidy
        # but don't build by default
        set_target_properties(pycer PROPERTIES EXCLUDE_FROM_ALL TRUE)
    endif()
endif()

#install(TARGETS core)
# TODO: Add the library to the filesystem, maybe with:
 # install(TARGETS core DESTINATION ~/.local/bin)

# /*       _\|/_
#          (o o)
#  +----oOO-{_}-OOo----+
#  |UNIT TESTS (Catch2)|
#  +------------------*/

find_package(Catch2 REQUIRED)
#
file(GLOB_RECURSE UNIT_TESTING_FILES
     src/tests/unit_tests/*.cpp
)

if(NOT CORE_ENABLE_MINTERM_OPTIMIZATION)
  # Those tests include Z3, which is not available in this configuration.
  list(FILTER UNIT_TESTING_FILES EXCLUDE REGEX "/optimizations/minterm_tree/")
endif()

file(GLOB_RECURSE THIRD_PARTY_LIBRARIES_TESTS
     src/tests/third_party_tests/*.cpp
)

add_executable(tests ${UNIT_TESTING_FILES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain core)
#
add_executable(third_party_tests ${THIRD_PARTY_LIBRARIES_TESTS})
target_link_libraries(third_party_tests PRIVATE Catch2::Catch2WithMain core)

# These tests need their own main (None at the moment)
#add_executable(custom-main-tests test.cpp test-main.cpp)
#target_link_libraries(custom-main-tests PRIVATE Catch2::Catch2)
