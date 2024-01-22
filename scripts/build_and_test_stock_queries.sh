#!/bin/bash

# Work at the root directory
# Should have conanfile.py present there.
cd "$(dirname "$0")"
cd ..

source scripts/common.sh
_setArgs "$@"

# Call build function from common
build

run_and_compare_script="scripts/run_and_compare.sh"
base_dir="src/targets/experiments/stocks"
executable="build/${BUILD_TYPE}/offline_experiment_stocks"

queries=$(find "$base_dir/queries" -type f) 

# FIX by having the port numbers be dynamic if already used
# parallel --verbose --halt now,fail=1 "$run_and_compare_script $executable '{} $base_dir/stock_data.csv' $base_dir/expected_results/{/}" ::: $queries

echo -e "Checking if expected_results folder exists"
if ! test -d $base_dir/expected_results; then
    echo -e "expected_results folder not found, uncompressing"
    tar -xf $base_dir/expected_results.tar.xz --directory $base_dir
fi

for query in $queries; do
    echo -e "Running ${query}"
    query_file=$(basename "$query")
    # if ! test -f "$base_dir/expected_results/$query_file"; then
    #     ./build/Debug/offline_experiment_stocks /home/nicobuzeta/Documents/CORE-Project/CORE/src/targets/experiments/stocks/queries/$query_file /home/nicobuzeta/Documents/CORE-Project/CORE/src/targets/experiments/stocks/stock_data.csv | tee /home/nicobuzeta/Documents/CORE-Project/CORE/src/targets/experiments/stocks/expected_results/$query_file
    # fi
    $run_and_compare_script $executable "$query $base_dir/stock_data.csv" "$base_dir/expected_results/$query_file"
    if [ $? -ne 0 ]; then
        echo -e "${RED}One or more queries did not match the expected results.${NORMAL_OUTPUT}"
        echo -e "${RED}Check if expected_results folder is up-to-date with tar.xz${NORMAL_OUTPUT}"
        exit 1
    fi
done
# Check if parallel succeeded

echo -e "${GREEN}All queries matched the expected results.${NORMAL_OUTPUT}"