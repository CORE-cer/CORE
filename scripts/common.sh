# Global array for excluded queries
declare -a EXCLUDED_QUERIES

function _setArgs() {
  while [ "${1:-}" != "" ]; do
    case "$1" in
      "-c" | "--compiler")
        shift
        COMPILER_PROFILE=$1
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
        EXCLUDED_QUERIES+=("$1")
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
  local triplet="x64-linux-${COMPILER_PROFILE}"
  local build_dir="build/${BUILD_TYPE}"

  # Determine parallel jobs
  local jobs
  if [ "$J" == "all-1" ]; then
    jobs=$(($(nproc) - 1))
    [ "$jobs" -lt 1 ] && jobs=1
  else
    jobs=$J
  fi

  # Sanitizer flags
  local sanitizer_flags=""
  if [ "$SANITIZER" == "address" ]; then
    sanitizer_flags="-DADDRESS_SANITIZER=ON -DTHREAD_SANITIZER=OFF"
  elif [ "$SANITIZER" == "thread" ]; then
    sanitizer_flags="-DTHREAD_SANITIZER=ON -DADDRESS_SANITIZER=OFF"
  else
    sanitizer_flags="-DADDRESS_SANITIZER=OFF -DTHREAD_SANITIZER=OFF"
  fi

  # Profiling flag
  local profiling_flag="OFF"
  if [ "$PROFILING" == "on" ]; then
    profiling_flag="ON"
  fi

  # GMP autotools needs C17 (C23 default breaks it)
  export CFLAGS="-std=gnu17"

  # Configure
  cmake -S . -B "${build_dir}" \
    -G Ninja \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
    -DCMAKE_TOOLCHAIN_FILE="vcpkg/scripts/buildsystems/vcpkg.cmake" \
    -DVCPKG_TARGET_TRIPLET="${triplet}" \
    -DVCPKG_OVERLAY_TRIPLETS="vcpkg-triplets" \
    -DLOGGING="${LOGGING}" \
    -DPROFILING="${profiling_flag}" \
    ${sanitizer_flags} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

  # Build
  cmake --build "${build_dir}" -j "${jobs}"

  build_result=$?
  if [ $build_result -ne 0 ]; then
      echo -e "${RED}Build failed!${NORMAL_OUTPUT}"
      exit 1
  fi
}

# Default values
BUILD_TYPE="Debug"
COMPILER_PROFILE="clang-libstdcxx"
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
