#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NORMAL_OUTPUT='\033[0m'

# Work at the root directory
cd "$(dirname "$0")"
cd ..

$HOME/.cargo/bin/run-clang-tidy clang-tidy-check.json -j
clang_tidy_result=$?



if [ $clang_tidy_result -eq 0 ]; then
    echo -e "${GREEN}clang-tidy passed!${NORMAL_OUTPUT}"
else
    echo -e "${RED}clang-tidy failed!${NORMAL_OUTPUT}"
    exit 1
fi
