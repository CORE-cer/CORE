#!/bin/bash

# Rebuild pycer Python bindings
# Forces a fresh build from C++ source
#
# Usage:
#   scripts/build_pycer.sh              # Normal build
#   scripts/build_pycer.sh -s address   # Build with address sanitizer
#   scripts/build_pycer.sh -s thread    # Build with thread sanitizer

cd "$(dirname "$0")"
cd ..

SANITIZER=""
while [ "${1:-}" != "" ]; do
  case "$1" in
    "-s" | "--sanitizer")
      shift
      SANITIZER=$1
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

uv sync --reinstall-package pycer $CONFIG_SETTINGS
