#!/bin/bash

# Work at the root directory
# Should have conanfile.py present there.
cd "$(dirname "$0")"
cd ../../..

source scripts/common.sh
_setArgs "$@"

# Call build function from common
build

run_and_compare_script="scripts/run_and_compare.sh"
base_dir="src/targets/experiments/taxis"
executable="build/${BUILD_TYPE}/offline"
csv="taxi_data.csv"
declaration="declaration.core"
compressed_csv="taxi_data.tar.xz"

queries=$(find "$base_dir/queries" -type f) 

# FIX by having the port numbers be dynamic if already used
# parallel --verbose --halt now,fail=1 "$run_and_compare_script $executable '{} $base_dir/stock_data.csv' $base_dir/expected_results/{/}" ::: $queries

echo -e "Checking if expected_results folder exists"
if ! test -d $base_dir/expected_results; then
    echo -e "expected_results folder not found, uncompressing"
    tar -xf $base_dir/expected_results.tar.xz --directory $base_dir
fi

echo -e "Checking if $csv exists"
if ! test -f $base_dir/$csv; then
    echo -e "$csv not found, uncompressing"
    tar -xf $base_dir/$compressed_csv --directory $base_dir
fi

for query in $queries; do
    echo -e "Running ${query}"
    query_file=$(basename "$query")
    # if ! test -f "$base_dir/expected_results/$query_file"; then
    #     { time $executable $base_dir/queries/$query_file $base_dir/$csv > $base_dir/expected_results/$query_file ; } 2> $base_dir/expected_results/"${query_file}_time.txt"
    # fi
    $run_and_compare_script $executable "-q $query -d $base_dir/$declaration -c $base_dir/$csv" "$base_dir/expected_results/$query_file"
    if [ $? -ne 0 ]; then
        rm -rf $base_dir/expected_results
        rm -f $base_dir/$csv
        echo -e "${RED}One or more queries did not match the expected results.${NORMAL_OUTPUT}"
        echo -e "${RED}Check if expected_results folder and $csv is up-to-date with tar.xz${NORMAL_OUTPUT}"
        exit 1
    fi
done
# Check if parallel succeeded

rm -rf $base_dir/expected_results
rm -f $base_dir/$csv
echo -e "${GREEN}All queries matched the expected results.${NORMAL_OUTPUT}"
