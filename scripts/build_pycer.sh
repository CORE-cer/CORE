#!/bin/bash

# Rebuild pycer Python bindings
# Forces a fresh build from C++ source
#
# Usage:
#   scripts/build_pycer.sh              # Normal build (Release)
#   scripts/build_pycer.sh -b Debug     # Debug build
#   scripts/build_pycer.sh -s address   # Build with address sanitizer
#   scripts/build_pycer.sh -s thread    # Build with thread sanitizer
#   scripts/build_pycer.sh -o           # Include the optional minterm-tree
#                                       # optimization (needs Z3; slow first build)

cd "$(dirname "$0")"
cd ..

SANITIZER=""
BUILD_TYPE=""
OPTIMIZATIONS=""
while [ "${1:-}" != "" ]; do
  case "$1" in
    "-s" | "--sanitizer")
      shift
      SANITIZER=$1
      ;;
    "-b" | "--build-type")
      shift
      BUILD_TYPE=$1
      ;;
    "-o" | "--optimizations")
      OPTIMIZATIONS=on
      ;;
  esac
  shift
done

CONFIG_SETTINGS=""
if [ "$SANITIZER" == "address" ]; then
  CONFIG_SETTINGS="--config-settings=cmake.define.ADDRESS_SANITIZER=ON"
elif [ "$SANITIZER" == "thread" ]; then
  CONFIG_SETTINGS="--config-settings=cmake.define.THREAD_SANITIZER=ON"
fi

if [ "$OPTIMIZATIONS" == "on" ]; then
  CONFIG_SETTINGS="$CONFIG_SETTINGS --config-settings=cmake.define.CORE_ENABLE_MINTERM_OPTIMIZATION=ON"
fi

if [ -n "$BUILD_TYPE" ]; then
  CONFIG_SETTINGS="$CONFIG_SETTINGS --config-settings=cmake.build-type=$BUILD_TYPE"
fi

uv sync --reinstall-package pycer $CONFIG_SETTINGS
