#!/bin/bash

# Single query runner that validates output
# Usage: run_single_query_and_check.sh <executable> <query_file> <declaration> <csv> <expected_output_file> [optional_args...]

# Work at the root directory
cd "$(dirname "$0")"
cd ..

source scripts/common.sh

# Check if at least 5 arguments are provided
if [ "$#" -lt 5 ]; then
    echo "Usage: $0 <executable> <query_file> <declaration> <csv> <expected_output_file> [optional_args...]"
    exit 1
fi

executable=$1
query_file=$2
declaration=$3
csv=$4
expected_output_file=$5
shift 5
optional_args=("$@")

temp_output_file=$(mktemp)
trap 'rm -f -- "$temp_output_file"' EXIT

# Run the executable with the arguments
$executable -q "$query_file" -d "$declaration" -c "$csv" "${optional_args[@]}" > "$temp_output_file"

executable_result=$?

if [ $executable_result -ne 0 ]; then
    echo -e "${RED}Query returned error code for $(basename $query_file)${NORMAL_OUTPUT}"
    rm -f -- "$temp_output_file"
    exit 1
fi

# Compare the actual output with the expected output
if diff -q "$temp_output_file" "$expected_output_file" >/dev/null; then
    echo -e "${GREEN}SUCCESS: $(basename "$query_file") output matches expected output.${NORMAL_OUTPUT}"
    rm -f -- "$temp_output_file"
    exit 0
else
    echo -e "${RED}FAILURE: $(basename "$query_file") output does not match expected output.${NORMAL_OUTPUT}"
    diff "$temp_output_file" "$expected_output_file"
    rm -f -- "$temp_output_file"
    exit 1
fi
