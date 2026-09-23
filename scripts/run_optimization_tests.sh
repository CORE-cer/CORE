#!/bin/bash

# Runs ONLY the optimization unit tests (Catch2 tag [Optimizations]) against an
# already-built test binary. It never builds, so it is quick when nothing has
# changed. After editing code, build first with
# scripts/build_and_test_optimizations.sh, or just re-run that script.
#
# Usage: scripts/run_optimization_tests.sh [-b Debug|Release]

# Work at the root directory
cd "$(dirname "$0")"
cd ..

source scripts/common.sh
_setArgs "$@"
OPTIMIZATIONS=on  # the optimization tests only exist in the optimization build

test_binary="$(get_build_dir)/tests"
if [ ! -x "$test_binary" ]; then
    echo -e "${RED}${test_binary} not found. Build it first with scripts/build_and_test_optimizations.sh${NORMAL_OUTPUT}"
    exit 1
fi

echo -e "${PURPLE}------------------------${NORMAL_OUTPUT}"
echo -e "${PURPLE}Optimization Unit Tests${NORMAL_OUTPUT}"
echo -e "${PURPLE}------------------------${NORMAL_OUTPUT}"
"./${test_binary}" "[Optimizations]"
optimization_test_result=$?

if [ $optimization_test_result -eq 0 ]; then
    echo -e "${GREEN}All optimization tests passed!${NORMAL_OUTPUT}"
else
    echo -e "${RED}Some optimization tests failed!${NORMAL_OUTPUT}"
    exit 1
fi
