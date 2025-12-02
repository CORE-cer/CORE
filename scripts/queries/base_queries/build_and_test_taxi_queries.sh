#!/bin/bash

# Work at the root directory
# Should have conanfile.py present there.
cd "$(dirname "$0")" || exit
cd ../../.. || exit

source scripts/common.sh
_setArgs "$@"

# Call build function from common
build

base_dir="src/targets/experiments/taxis"
executable="build/offline"
csv="taxi_data.csv"
declaration="declaration.core"
compressed_csv="taxi_data.tar.xz"

queries=$(find "$base_dir/queries" -type f | sort -V)

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
    scripts/run_single_query_and_check.sh "$executable" "$query" "$base_dir/$declaration" "$base_dir/$csv" "$base_dir/expected_results/$query_file"
    if [ $? -ne 0 ]; then
        if [ -n "$base_dir" ] && [[ "$base_dir" == "src/targets/experiments/taxis" ]]; then
            rm -rf "$base_dir/expected_results"
            rm -f "$base_dir/$csv"
        else
            echo "Aborting rm: base_dir is not set or is outside the expected directory."
        fi
        echo -e "${RED}One or more queries did not match the expected results.${NORMAL_OUTPUT}"
        echo -e "${RED}Check if expected_results folder and $csv is up-to-date with tar.xz${NORMAL_OUTPUT}"
        exit 1
    fi
done

if [ -n "$base_dir" ] && [[ "$base_dir" == "src/targets/experiments/taxis" ]]; then
    rm -rf "$base_dir/expected_results"
    rm -f "$base_dir/$csv"
else
    echo "Aborting rm: base_dir is not set or is outside the expected directory."
fi
echo -e "${GREEN}All queries matched the expected results.${NORMAL_OUTPUT}"
