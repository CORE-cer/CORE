#!/bin/bash

# Work at the root directory
# Should have conanfile.py present there.
cd "$(dirname "$0")"
cd ..

source scripts/common.sh

# Check if exactly three arguments are provided
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <executable> <'arguments'> <expected_output_file>"
    exit 1
fi

executable=$1
arguments=$2
expected_output_file=$3

# Run the executable with the arguments and capture the output
actual_output=$( $executable $arguments )

# Compare the actual output with the expected output
if diff <(echo "$actual_output") $expected_output_file > /dev/null; then
    echo -e "${GREEN}Output matches expected of ${expected_output_file}${NORMAL_OUTPUT}"
else
    echo -e "${RED}Output does not match expected of ${expected_output_file}${NORMAL_OUTPUT}"
    exit 1
fi
