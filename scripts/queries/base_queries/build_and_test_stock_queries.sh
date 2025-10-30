#!/bin/bash

# Work at the root directory
# Should have conanfile.py present there.
cd "$(dirname "$0")"
cd ../../..

source scripts/common.sh
_setArgs "$@"

# Call build function from common
build

base_dir="src/targets/experiments/stocks"
executable="build/${BUILD_TYPE}/offline"
csv="stock_data.csv"
declaration="declaration.core"

queries=$(find "$base_dir/queries" -type f | sort -V)

echo -e "Checking if expected_results folder exists"
if ! test -d $base_dir/expected_results; then
    echo -e "expected_results folder not found, uncompressing"
    tar -xf $base_dir/expected_results.tar.xz --directory $base_dir
fi

for query in $queries; do
    echo -e "Running ${query}"
    query_file=$(basename "$query")
    scripts/run_single_query_and_check.sh "$executable" "$query" "$base_dir/$declaration" "$base_dir/$csv" "$base_dir/expected_results/$query_file"
    if [ $? -ne 0 ]; then
        rm -rf $base_dir/expected_results
        echo -e "${RED}One or more queries did not match the expected results.${NORMAL_OUTPUT}"
        echo -e "${RED}Check if expected_results folder is up-to-date with tar.xz${NORMAL_OUTPUT}"
        exit 1
    fi
done

rm -rf $base_dir/expected_results
echo -e "${GREEN}All queries matched the expected results.${NORMAL_OUTPUT}"
