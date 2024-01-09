cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

project (core CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src/)

find_package(antlr4-runtime REQUIRED)
find_package(cereal REQUIRED)
find_package(cppzmq REQUIRED)
find_package(libpqxx REQUIRED)
find_package(gmp REQUIRED)
find_package(re2 REQUIRED)


# Profiler
message("Fetching Tracy")

Set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare (
  tracy
  GIT_REPOSITORY https://github.com/wolfpld/tracy.git
  GIT_TAG master
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)

set(ENABLE_PROFILING OFF CACHE BOOL "Enable Profiling" FORCE)

if (ENABLE_PROFILING)
  set(TRACY_ENABLE ON CACHE BOOL "Enable Tracy" FORCE)
  set(TRACY_NO_EXIT ON CACHE BOOL "No Exit Tracy" FORCE)
  set(TRACY_DEBUGINFOD ON CACHE BOOL "Use DEBUGINFOD Tracy" FORCE)
else()
  set(TRACY_ENABLE OFF CACHE BOOL "Disable Tracy" FORCE)
endif()

FetchContent_MakeAvailable (tracy)

# Target in Makefile for generation of grammar from .g4 file.
add_custom_target(
  grammar
  COMMAND ${CMAKE_SOURCE_DIR}/src/core_server/internal/parsing/ceql_query/autogenerate_script.sh
  COMMAND ${CMAKE_SOURCE_DIR}/src/core_server/internal/parsing/stream_declaration/autogenerate_script.sh
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

add_library(core STATIC
            ${SHARED_FILES}
            ${CLIENT_FILES}
            ${SERVER_FILES}
            ${STREAMER_FILES})

target_link_libraries(core PUBLIC antlr4_static cereal cppzmq gmp::gmp gmp::gmpxx gmp::libgmp ${libpqxx_LIBRARIES} -pthread re2::re2 TracyClient)
if (ENABLE_PROFILING)
  target_link_libraries(core PUBLIC debuginfod)
endif()

set_target_properties(core PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

# Set debug flag for core
if(CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_definitions(core PUBLIC CORE_DEBUG)
endif()

# Add compiler and linker flags to core
target_compile_options(core PUBLIC -Werror=old-style-cast)

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


# executable target

add_executable(online_simulation1 src/targets/online/simulation1.cpp)
add_executable(online_simulation2 src/targets/online/simulation2.cpp)
add_executable(online_polkura_example src/targets/online/polkura_example/polkura.cpp)
add_executable(online_stocks_example src/targets/online/stocks_example/stocks_example.cpp)
add_executable(online_taxi_example src/targets/online/taxi_example/taxi_example.cpp)
add_executable(online_client src/targets/online/client.cpp)
add_executable(online_server src/targets/online/server.cpp)

target_link_libraries(online_simulation1 PRIVATE core)
target_link_libraries(online_simulation2 PRIVATE core)
target_link_libraries(online_polkura_example PRIVATE core)
target_link_libraries(online_stocks_example PRIVATE core)
target_link_libraries(online_taxi_example PRIVATE core)
target_link_libraries(online_client PRIVATE core)
target_link_libraries(online_server PRIVATE core)



add_executable(offline_simulation1 src/targets/online/simulation1.cpp)
add_executable(offline_simulation2 src/targets/online/simulation2.cpp)
add_executable(offline_polkura_example src/targets/online/polkura_example/polkura.cpp)
add_executable(offline_stocks_example src/targets/online/stocks_example/stocks_example.cpp)
add_executable(offline_taxi_example src/targets/online/taxi_example/taxi_example.cpp)
add_executable(offline_client src/targets/online/client.cpp)
add_executable(offline_server src/targets/online/server.cpp)

target_link_libraries(offline_simulation1 PRIVATE core)
target_link_libraries(offline_simulation2 PRIVATE core)
target_link_libraries(offline_polkura_example PRIVATE core)
target_link_libraries(offline_stocks_example PRIVATE core)
target_link_libraries(offline_taxi_example PRIVATE core)
target_link_libraries(offline_client PRIVATE core)
target_link_libraries(offline_server PRIVATE core)

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
     src/unit_tests/*.cpp
)

file(GLOB_RECURSE THIRD_PARTY_LIBRARIES_TESTS
     src/third_party_tests/*.cpp
)

add_executable(tests ${UNIT_TESTING_FILES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain core)
#
add_executable(third_party_tests ${THIRD_PARTY_LIBRARIES_TESTS})
target_link_libraries(third_party_tests PRIVATE Catch2::Catch2WithMain core)

# These tests need their own main (None at the moment)
#add_executable(custom-main-tests test.cpp test-main.cpp)
#target_link_libraries(custom-main-tests PRIVATE Catch2::Catch2)
