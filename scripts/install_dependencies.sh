#!/bin/bash

# Work at the root directory
# Should have vcpkg.json present there.
cd "$(dirname "$0")"
cd ..

# Include common
source scripts/common.sh
_setArgs "$@"

# Check if vcpkg is installed
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
    echo -e "${PURPLE}Please install vcpkg and set VCPKG_ROOT environment variable.${NORMAL_OUTPUT}"
    echo -e "${PURPLE}Installation instructions: https://github.com/microsoft/vcpkg${NORMAL_OUTPUT}"
    exit 1
  fi
fi

echo -e "${GREEN}Found vcpkg at: ${VCPKG_ROOT}${NORMAL_OUTPUT}"

# Ensure vcpkg is bootstrapped
if [ ! -f "${VCPKG_ROOT}/vcpkg" ]; then
  echo -e "${PURPLE}vcpkg executable not found. Running bootstrap...${NORMAL_OUTPUT}"
  "${VCPKG_ROOT}/bootstrap-vcpkg.sh"
fi

echo -e "${GREEN}vcpkg is ready!${NORMAL_OUTPUT}"
echo -e "${PURPLE}Dependencies will be installed automatically during CMake configuration.${NORMAL_OUTPUT}"
echo -e "${PURPLE}To manually install dependencies, run: ${VCPKG_ROOT}/vcpkg install${NORMAL_OUTPUT}"
