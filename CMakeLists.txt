cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (core CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src/)

find_package(antlr4-runtime REQUIRED)
find_package(cereal REQUIRED)
find_package(ZeroMQ REQUIRED)
find_package(libpqxx REQUIRED)

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

target_link_libraries(core antlr4_static cereal ${ZeroMQ_LIBRARIES} ${libpqxx_LIBRARIES} -pthread )

set_target_properties(core PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

# executable target

add_executable(simulation src/targets/main.cpp)
add_executable(client src/targets/client.cpp)
add_executable(server src/targets/server.cpp)

target_link_libraries(simulation core)
target_link_libraries(client core)
target_link_libraries(server core)

#install(TARGETS core)
# TODO: Add the library to the filesystem, maybe with:
 install(TARGETS core DESTINATION ~/.local/bin)

# /*       _\|/_
#          (o o)
#  +----oOO-{_}-OOo----+
#  |UNIT TESTS (Catch2)|
#  +------------------*/

find_package(Catch2 REQUIRED)

file(GLOB_RECURSE UNIT_TESTING_FILES
     src/unit_tests/*.cpp
)

file(GLOB_RECURSE THIRD_PARTY_LIBRARIES_TESTS
     src/third_party_tests/*.cpp
)

add_executable(tests ${UNIT_TESTING_FILES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain core)

add_executable(third_party_tests ${THIRD_PARTY_LIBRARIES_TESTS})
target_link_libraries(third_party_tests PRIVATE Catch2::Catch2WithMain core)

# These tests need their own main (None at the moment)
#add_executable(custom-main-tests test.cpp test-main.cpp)
#target_link_libraries(custom-main-tests PRIVATE Catch2::Catch2)
