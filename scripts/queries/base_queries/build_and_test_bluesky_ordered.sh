#!/bin/bash

# Work at the root directory
# Should have conanfile.py present there.
cd "$(dirname "$0")" || exit
cd ../../.. || exit

source scripts/common.sh
_setArgs "$@"

# Call build function from common
build

base_dir="src/targets/experiments/ordered_bluesky"
executable="build/offline"
csv="bluesky_ordered.corecsv"
declaration="declaration.core"
quarantine_declaration="quarantine_declaration.core"

queries=$(find "$base_dir/queries" -type f | sort -V)

echo -e "Checking if expected_results folder exists"
if ! test -d $base_dir/expected_results; then
    echo -e "expected_results folder not found, uncompressing"
    tar -xf $base_dir/expected_results.tar.xz --directory $base_dir
fi

for query in $queries; do
    echo -e "Running ${query}"
    query_file=$(basename "$query")
    scripts/run_single_query_and_check.sh "$executable" "$query" "$base_dir/$declaration" "$base_dir/$csv" "$base_dir/expected_results/$query_file" "-o" "$base_dir/$quarantine_declaration"
    if [ $? -ne 0 ]; then
        if [ -n "$base_dir" ] && [[ "$base_dir" == "src/targets/experiments/ordered_bluesky" ]]; then
            rm -rf "$base_dir/expected_results"
        else
            echo "Aborting rm -rf: base_dir is not set or is outside the expected directory."
        fi
        echo -e "${RED}One or more queries did not match the expected results.${NORMAL_OUTPUT}"
        echo -e "${RED}Check if expected_results folder is up-to-date with tar.xz${NORMAL_OUTPUT}"
        exit 1
    fi
done

if [ -n "$base_dir" ] && [[ "$base_dir" == "src/targets/experiments/ordered_bluesky" ]]; then
    rm -rf "$base_dir/expected_results"
else
    echo "Aborting rm -rf: base_dir is not set or is outside the expected directory."
fi
echo -e "${GREEN}All queries matched the expected results.${NORMAL_OUTPUT}"
