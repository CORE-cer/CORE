# Global array for excluded queries
declare -a EXCLUDED_QUERIES

function _setArgs() {
  while [ "${1:-}" != "" ]; do
    case "$1" in
      "-p" | "--profile")
        shift
        VCPKG_PROFILE=$1
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

# Map profile path to vcpkg triplet and toolchain
function get_vcpkg_triplet() {
  local profile=$1

  # Strip any path prefix (e.g., vcpkg_triplets/ or conan_profiles/)
  profile=$(basename "$profile")

  case "$profile" in
    *"x86_64-linux-clang-libstdc"*)
      echo "x64-linux-clang-libstdcxx"
      ;;
    *"x86_64-linux-gcc-libstdc"*)
      echo "x64-linux-gcc-libstdcxx"
      ;;
    *"aarch64-linux-gcc-libstdc"*)
      echo "arm64-linux-gcc-libstdcxx"
      ;;
    *"aarch64-macos-clang-libc"*)
      echo "arm64-osx-clang-libcxx"
      ;;
    *)
      # If profile is already a triplet name, return as-is
      echo "$profile"
      ;;
  esac
}

function build() {
  # Get vcpkg triplet from profile
  VCPKG_TRIPLET=$(get_vcpkg_triplet "${VCPKG_PROFILE}")

  # Set up build directory
  BUILD_DIR="build/${BUILD_TYPE}"
  mkdir -p "${BUILD_DIR}"

  # Set vcpkg root (adjust if vcpkg is installed elsewhere)
  if [ -z "${VCPKG_ROOT:-}" ]; then
    # Try common vcpkg locations
    if [ -d "${HOME}/vcpkg" ]; then
      export VCPKG_ROOT="${HOME}/vcpkg"
    elif [ -d "/usr/local/vcpkg" ]; then
      export VCPKG_ROOT="/usr/local/vcpkg"
    elif [ -d "${PWD}/vcpkg" ]; then
      export VCPKG_ROOT="${PWD}/vcpkg"
    else
      echo -e "${RED}VCPKG_ROOT not set and vcpkg not found in common locations.${NORMAL_OUTPUT}"
      echo -e "${RED}Please set VCPKG_ROOT environment variable or install vcpkg.${NORMAL_OUTPUT}"
      exit 1
    fi
  fi

  # Set up CMake variables based on sanitizer option
  CMAKE_SANITIZER_FLAGS=""
  if [ "$SANITIZER" = "address" ]; then
    CMAKE_SANITIZER_FLAGS="-DADDRESS_SANITIZER=ON -DTHREAD_SANITIZER=OFF"
  elif [ "$SANITIZER" = "thread" ]; then
    CMAKE_SANITIZER_FLAGS="-DADDRESS_SANITIZER=OFF -DTHREAD_SANITIZER=ON"
  else
    CMAKE_SANITIZER_FLAGS="-DADDRESS_SANITIZER=OFF -DTHREAD_SANITIZER=OFF"
  fi

  # Set up profiling flag
  CMAKE_PROFILING_FLAG="-DPROFILING=OFF"
  if [ "$PROFILING" = "on" ]; then
    CMAKE_PROFILING_FLAG="-DPROFILING=ON"
  fi

  # Calculate number of jobs
  if [ "$J" = "all-1" ]; then
    NJOBS=$(($(nproc) - 1))
  else
    NJOBS="$J"
  fi

  # Find Ninja executable
  NINJA_PATH=$(which ninja)
  if [ -z "$NINJA_PATH" ]; then
    echo -e "${RED}Ninja is required but not found in PATH!${NORMAL_OUTPUT}"
    exit 1
  fi

  # Verify vcpkg toolchain file exists
  VCPKG_TOOLCHAIN="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
  if [ ! -f "${VCPKG_TOOLCHAIN}" ]; then
    echo -e "${RED}vcpkg toolchain file not found at: ${VCPKG_TOOLCHAIN}${NORMAL_OUTPUT}"
    echo -e "${RED}Please ensure vcpkg is properly installed and bootstrapped.${NORMAL_OUTPUT}"
    echo -e "${PURPLE}Run: ${VCPKG_ROOT}/bootstrap-vcpkg.sh${NORMAL_OUTPUT}"
    exit 1
  fi

  # Configure with vcpkg
  echo -e "${GREEN}Configuring with CMake and vcpkg...${NORMAL_OUTPUT}"
  cmake -S . -B "${BUILD_DIR}" \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
    -DCMAKE_MAKE_PROGRAM="${NINJA_PATH}" \
    -DCMAKE_TOOLCHAIN_FILE="${VCPKG_TOOLCHAIN}" \
    -DVCPKG_TARGET_TRIPLET="${VCPKG_TRIPLET}" \
    -DVCPKG_OVERLAY_TRIPLETS="${PWD}/vcpkg_triplets" \
    -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE="${PWD}/vcpkg_toolchains/${VCPKG_TRIPLET}.cmake" \
    ${CMAKE_SANITIZER_FLAGS} \
    ${CMAKE_PROFILING_FLAG} \
    -DLOGGING="${LOGGING}" \
    -GNinja

  configure_result=$?
  if [ $configure_result -ne 0 ]; then
    echo -e "${RED}CMake configuration failed!${NORMAL_OUTPUT}"
    exit 1
  fi

  # Build with ninja
  echo -e "${GREEN}Building with Ninja (using ${NJOBS} jobs)...${NORMAL_OUTPUT}"
  cmake --build "${BUILD_DIR}" -j "${NJOBS}"

  build_result=$?
  if [ $build_result -ne 0 ]; then
    echo -e "${RED}Build failed!${NORMAL_OUTPUT}"
    exit 1
  fi
}

# Default values
BUILD_TYPE="Debug"
VCPKG_PROFILE="x64-linux-clang-libstdcxx"
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
