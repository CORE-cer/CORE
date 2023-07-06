#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
PURPLE='\033[0;35m'
NORMAL_OUTPUT='\033[0m'

# Change to parent directory
cd "$(dirname "$0")/.."

# Compile the project under "Debug"
./scripts/build.sh

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
