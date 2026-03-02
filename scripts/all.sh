#!/bin/bash

# Work at the root directory
cd "$(dirname "$0")" || exit
cd .. || exit

for buildType in "Debug" "Release"; do

    echo -e "${GREEN}Running ${buildType}${NORMAL_OUTPUT}"

    if [ $buildType == "Debug" ]; then
        echo -e "${GREEN}Running clang-format${NORMAL_OUTPUT}"
        ./scripts/clang_format_all_files.sh "$@ -b ${buildType}" &> /dev/null
        result=$?
        if [ $result -ne 0 ]; then
            echo -e "${RED}Some files were not formatted correctly, Fail! (${buildType})${NORMAL_OUTPUT}"
            exit 1
        fi

        echo -e "${GREEN}Running clang-tidy${NORMAL_OUTPUT}"
        ./scripts/clang_tidy_check_all_files.sh "$@ -b ${buildType}" &> /dev/null
        result=$?
        if [ $result -ne 0 ]; then
            echo -e "${RED}Clang Tidy Failed! (${buildType})${NORMAL_OUTPUT}"
            exit 1
        fi
    fi

    echo -e "${GREEN}Running build${NORMAL_OUTPUT}"
    ./scripts/build.sh "$@ -b ${buildType}" &> /dev/null
    result=$?
    if [ $result -ne 0 ]; then
        echo -e "${RED}Build Failed! (${buildType})${NORMAL_OUTPUT}"
        exit 1
    fi

    echo -e "${GREEN}Running build and test${NORMAL_OUTPUT}"
    ./scripts/build_and_test.sh "$@ -b ${buildType}" &> /dev/null
    result=$?
    if [ $result -ne 0 ]; then
        echo -e "${RED}Some tests failed! (${buildType})${NORMAL_OUTPUT}"
        exit 1
    fi

    echo -e "${GREEN}Running build and valgrind${NORMAL_OUTPUT}"
    ./scripts/build_and_valgrind.sh "$@ -b ${buildType}" &> /dev/null
    result=$?
    if [ $result -ne 0 ]; then
        echo -e "${RED}Some tests failed with valgrind! (${buildType})${NORMAL_OUTPUT}"
        exit 1
    fi

    # Integration query tests are now run via: uv run pytest tests/e2e/ -v

done
