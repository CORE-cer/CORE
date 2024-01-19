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
executable="build/Debug/offline_experiment_stocks"

queries=$(find "$base_dir/queries" -type f) 

# FIX by having the port numbers be dynamic if already used
# parallel --verbose --halt now,fail=1 "$run_and_compare_script $executable '{} $base_dir/stock_data.csv' $base_dir/expected_results/{/}" ::: $queries

for query in $queries; do
    echo -e "Running ${query}"
    query_file=$(basename "$query")
    $run_and_compare_script $executable "$query $base_dir/stock_data.csv" "$base_dir/expected_results/$query_file"
    if [ $? -ne 0 ]; then
        echo "One or more queries did not match the expected results."
        exit 1
    fi
done
# Check if parallel succeeded

echo "All queries matched the expected results."
