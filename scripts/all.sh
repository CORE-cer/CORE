#!/bin/bash

# Work at the root directory
# Should have conanfile.py present there.
cd "$(dirname "$0")"
cd ..

for buildType in "Debug" "Release"; do

    echo -e "${GREEN}Running ${buildType}${NORMAL_OUTPUT}"

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

    if [ $buildType == "Release" ]; then

        echo -e "${GREEN}Running build and test stock queries${NORMAL_OUTPUT}"
        ./scripts/build_and_test_stock_queries.sh "$@ -b ${buildType}"
        result=$?
        if [ $result -ne 0 ]; then
            echo -e "${RED}Some stock queries failed! (${buildType})${NORMAL_OUTPUT}"
            exit 1
        fi

        echo -e "${GREEN}Running build and test smart homes queries${NORMAL_OUTPUT}"
        ./scripts/build_and_test_smart_homes_queries.sh "$@ -b ${buildType}"
        result=$?
        if [ $result -ne 0 ]; then
            echo -e "${RED}Some smart homes queries failed! (${buildType})${NORMAL_OUTPUT}"
            exit 1
        fi

        echo -e "${GREEN}Running build and test taxi queries${NORMAL_OUTPUT}"
        ./scripts/build_and_test_taxi_queries.sh "$@ -b ${buildType}"
        result=$?
        if [ $result -ne 0 ]; then
            echo -e "${RED}Some taxi queries failed! (${buildType})${NORMAL_OUTPUT}"
            exit 1
        fi
    fi

done
