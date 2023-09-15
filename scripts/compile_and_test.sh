#!/bin/bash

# Work at the root directory
# Should have conanfile.py present there.
cd "$(dirname "$0")"
cd ..

# Include common
# source scripts/common.sh
_setArgs "$@"

# Compile the project under "Debug"
./scripts/build.sh --build_type=Debug "$@"
build_result=$?

if [ $build_result -ne 0 ]; then
    echo -e "${RED}Build failed!${NORMAL_OUTPUT}"
    exit 1
fi

echo -e "${PURPLE}--------------${NORMAL_OUTPUT}"
echo -e "${PURPLE}Internal Tests${NORMAL_OUTPUT}"
echo -e "${PURPLE}--------------${NORMAL_OUTPUT}"
./build/Debug/tests
unit_test_result=$?

echo -e "${PURPLE}-------------------------${NORMAL_OUTPUT}"
echo -e "${PURPLE}Third Party Library Tests${NORMAL_OUTPUT}"
echo -e "${PURPLE}-------------------------${NORMAL_OUTPUT}"
./build/Debug/third_party_tests
third_party_test_result=$?

if [ $unit_test_result -eq 0 ] && [ $third_party_test_result -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NORMAL_OUTPUT}"
else
    echo -e "${RED}Some tests failed!${NORMAL_OUTPUT}"
    exit 1
fi
