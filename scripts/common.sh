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
        # Validate BUILD_TYPE
        case "$BUILD_TYPE" in
          Debug|Release|RelWithDebInfo|MinSizeRel)
            ;; # Valid build type
          *)
            echo -e "${RED}Error: Invalid build type '${BUILD_TYPE}'. Valid options are: Debug, Release, RelWithDebInfo, MinSizeRel${NORMAL_OUTPUT}"
            exit 1
            ;;
        esac
        ;;
      "-s" | "--sanitizer")
        shift
        SANITIZER=$1
        # Validate SANITIZER
        case "$SANITIZER" in
          none|address|thread)
            ;; # Valid sanitizer
          *)
            echo -e "${RED}Error: Invalid sanitizer '${SANITIZER}'. Valid options are: none, address, thread${NORMAL_OUTPUT}"
            exit 1
            ;;
        esac
        ;;
      "--exclude")
        shift
        EXCLUDED_QUERIES+=("$1") # Add the excluded query to the array
        ;;
      "-l"| "--logging")
        shift
        LOGGING="$1"
        # Validate LOGGING
        case "$LOGGING" in
          trace|debug|info|warn|error|critical|off)
            ;; # Valid logging level
          *)
            echo -e "${RED}Error: Invalid logging level '${LOGGING}'. Valid options are: trace, debug, info, warn, error, critical, off${NORMAL_OUTPUT}"
            exit 1
            ;;
        esac
        ;;
      "-j")
        shift
        J="$1"
        # Validate J (should be a positive number or "all-1")
        if [ "$J" != "all-1" ] && ! [[ "$J" =~ ^[0-9]+$ ]]; then
          echo -e "${RED}Error: Invalid value for -j flag '${J}'. Must be a positive number or 'all-1'${NORMAL_OUTPUT}"
          exit 1
        fi
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
