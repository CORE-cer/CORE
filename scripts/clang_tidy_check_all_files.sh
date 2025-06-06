#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NORMAL_OUTPUT='\033[0m'

# Work at the root directory
cd "$(dirname "$0")"
cd ..

source scripts/common.sh
_setArgs "$@"

# Call build function from common
build

files=$(find src -name "*.hpp" -o -name "*.cpp" ! -path "*/autogenerated/*")


if [[ $(uname) == "Darwin" ]] ; then
    number_of_cores=$(sysctl -n hw.logicalcpu)
else 
    number_of_cores=$(nproc --all)
fi

parallel --bar --jobs $number_of_cores --halt now,fail=1 clang-tidy -p build/compile_commands.json --header-filter=".*\/src\/.*" -system-headers ::: $files
clang_tidy_result=$?



if [ $clang_tidy_result -eq 0 ]; then
    echo -e "${GREEN}clang-tidy passed!${NORMAL_OUTPUT}"
else
    echo -e "${RED}clang-tidy failed!${NORMAL_OUTPUT}"
    exit 1
fi
