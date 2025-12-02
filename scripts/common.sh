# Global array for excluded queries
declare -a EXCLUDED_QUERIES

function _setArgs() {
  while [ "${1:-}" != "" ]; do
    case "$1" in
      "-p" | "--profile")
        shift
        CONAN_PROFILE=$1
        ;;
      "-b" | "--buildType")
        shift
        BUILD_TYPE=$1
        ;;
      "-s" | "--sanitizer")
        shift
        SANITIZER=$1
        ;;
      "--exclude")
        shift
        EXCLUDED_QUERIES+=("$1") # Add the excluded query to the array
        ;;
      "-l"| "--logging")
        shift
        LOGGING="$1"
        ;;
      "-j")
        shift
        J="$1"
        ;;
      "--profiling")
        PROFILING=on
        ;;
    esac
    shift
  done
}

function build() {
  # Define CMake flags based on arguments
  CMAKE_FLAGS="-DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake"

  if [ "${SANITIZER}" == "address" ]; then
      CMAKE_FLAGS="${CMAKE_FLAGS} -DADDRESS_SANITIZER=ON"
  elif [ "${SANITIZER}" == "thread" ]; then
      CMAKE_FLAGS="${CMAKE_FLAGS} -DTHREAD_SANITIZER=ON"
  fi

  # Logging
  CMAKE_FLAGS="${CMAKE_FLAGS} -DLOGGING=${LOGGING}"

  # Profiling
  if [ "${PROFILING}" == "on" ]; then
      CMAKE_FLAGS="${CMAKE_FLAGS} -DPROFILING=ON"
  fi

  # Configure
  cmake -B build -S . -G Ninja ${CMAKE_FLAGS}

  # Build
  if [ "${J}" == "all-1" ]; then
      # Get number of cores - 1
      CORES=$(nproc)
      CORES=$((CORES-1))
      cmake --build build -j${CORES}
  else
      cmake --build build -j${J}
  fi

  build_result=$?
  if [ $build_result -ne 0 ]; then
      echo -e "${RED}Build failed!${NORMAL_OUTPUT}"
      exit 1
  fi
  # echo current path
}

# Default values
BUILD_TYPE="Debug"
CONAN_PROFILE="conan_profiles/x86_64-linux-clang-libstdc"
SANITIZER=none
LOGGING=info
J="all-1"
PROFILING=off
export TSAN_OPTIONS="suppressions=tsan_suppressions.txt"
export ASAN_OPTIONS="suppressions=asan_suppressions.txt"

RED='\033[0;31m'
GREEN='\033[0;32m'
PURPLE='\033[0;35m'
NORMAL_OUTPUT='\033[0m'
