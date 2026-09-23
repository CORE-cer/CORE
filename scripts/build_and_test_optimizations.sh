#!/bin/bash

# Builds the project WITH the optional optimizations (minterm-tree, which needs
# Z3) and runs only the optimization unit tests, i.e. the Catch2 test cases
# tagged [Optimizations] (src/tests/unit_tests/core_server/internal/optimizations/).
#
# The build goes to its own directory (build/<type>-opt), so it does not disturb
# your regular build. The first run compiles Z3 from source, which is slow.
# When everything is already built, use scripts/run_optimization_tests.sh
# instead: it skips the build step.
#
# Usage: scripts/build_and_test_optimizations.sh [-b Debug|Release] [-s address|thread] [-j N]

# Work at the root directory
cd "$(dirname "$0")"
cd ..

source scripts/common.sh
_setArgs "$@"
OPTIMIZATIONS=on  # this script is about the optimizations: always build them

# Call build function from common
build
BUILD_DIR="$(get_build_dir)"

echo -e "${PURPLE}------------------------${NORMAL_OUTPUT}"
echo -e "${PURPLE}Optimization Unit Tests${NORMAL_OUTPUT}"
echo -e "${PURPLE}------------------------${NORMAL_OUTPUT}"
./${BUILD_DIR}/tests "[Optimizations]"
optimization_test_result=$?

if [ $optimization_test_result -eq 0 ]; then
    echo -e "${GREEN}All optimization tests passed!${NORMAL_OUTPUT}"
else
    echo -e "${RED}Some optimization tests failed!${NORMAL_OUTPUT}"
    exit 1
fi
