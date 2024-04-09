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

temp_output_file=$(mktemp)
trap 'rm -f -- "$temp_output_file"' EXIT

# Run the executable with the arguments and pipe input into temporary file
$executable $arguments > $temp_output_file

echo "output of temp file:"
cat $temp_output_file
echo "expected values:"
cat $expected_output_file

# Compare the actual output with the expected output
if diff $temp_output_file $expected_output_file > /dev/null; then
    echo -e "${GREEN}Output matches expected of ${expected_output_file}${NORMAL_OUTPUT}"
else
    echo -e "${RED}Output does not match expected of ${expected_output_file}${NORMAL_OUTPUT}"
    exit 1
fi
