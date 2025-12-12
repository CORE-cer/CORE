#!/bin/bash

# Work at the root directory
cd "$(dirname "$0")"
cd ..

source scripts/common.sh
_setArgs "$@"

# Call build function from common
build

echo -e "${PURPLE}--------------${NORMAL_OUTPUT}"
echo -e "${PURPLE}Internal Tests with Valgrind${NORMAL_OUTPUT}"
echo -e "${PURPLE}--------------${NORMAL_OUTPUT}"

valgrind --leak-check=full --suppressions=./valgrind_suppressions.txt \
    --exit-on-first-error=yes --error-exitcode=1 --gen-suppressions=all ./build/tests
valgrind_unit_test_result=$?


# Commented out third party tests for now
# valgrind added delay makes tests fails due
# to threads taking to long to connect to using subscriptions

# echo -e "${PURPLE}-------------------------${NORMAL_OUTPUT}"
# echo -e "${PURPLE}Third Party Library Tests${NORMAL_OUTPUT}"
# echo -e "${PURPLE}-------------------------${NORMAL_OUTPUT}"
# valgrind --leak-check=full --suppressions=./valgrind_suppressions.txt \
#     --exit-on-first-error=yes --error-exitcode=1 ./build/third_party_tests
# valgrind_third_party_test_result=$?
valgrind_third_party_test_result=0

if [ $valgrind_unit_test_result -eq 0 ] && [ $valgrind_third_party_test_result -eq 0 ]; then
    echo -e "${GREEN}Valgrind passed!${NORMAL_OUTPUT}"
else
    echo -e "${RED}Some tests failed!${NORMAL_OUTPUT}"
    exit 1
fi


