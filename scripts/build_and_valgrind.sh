#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
PURPLE='\033[0;35m'
NORMAL_OUTPUT='\033[0m'

# Change to parent directory
cd "$(dirname "$0")/.."

# Compile the project under "Debug"
./scripts/build.sh
build_result=$?

if [ $build_result -ne 0 ]; then
    echo -e "${RED}Build failed!${NORMAL_OUTPUT}"
    exit 1
fi

echo -e "${PURPLE}--------------${NORMAL_OUTPUT}"
echo -e "${PURPLE}Internal Tests with Valgrind${NORMAL_OUTPUT}"
echo -e "${PURPLE}--------------${NORMAL_OUTPUT}"
valgrind --leak-check=full --suppressions=./valgrind_suppressions.txt \
    --exit-on-first-error=yes --error-exitcode=1  ./build/Debug/tests
valgrind_test_result=$?

if [ $valgrind_test_result -eq 0 ]; then
    echo -e "${GREEN}Valgrind passed!${NORMAL_OUTPUT}"
else
    echo -e "${RED}Some tests failed!${NORMAL_OUTPUT}"
    exit 1
fi
