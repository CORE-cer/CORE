#!/bin/bash

# Per-query benchmark runner
# Benchmarks each query individually with validation
#
# Configuration via environment variables:
# BENCHMARK_BASE_DIR: Base directory for experiment (required)
# BENCHMARK_FILE: Output CSV file (default: $BENCHMARK_BASE_DIR/benchmark_${BUILD_TYPE}.csv)
# BENCHMARK_REPEATS: Number of runs per query (default: 3)
# BENCHMARK_EXECUTABLE: Path to executable (default: build/${BUILD_TYPE}/offline)
# BENCHMARK_CSV: Data CSV file (required)
# BENCHMARK_DECLARATION: Declaration file (required)
# BENCHMARK_EXTRA_ARGS: Extra arguments (e.g., "-o quarantine.core")
# BENCHMARK_COMPRESSED_CSV: Compressed CSV file to extract (optional)
# BENCHMARK_CLEANUP_CSV: Clean up CSV after (optional, default: false)
# BENCHMARK_COMPRESSED_RESULTS: Compressed expected_results to extract (optional)

cd "$(dirname "$0")"
cd ../../..

source scripts/common.sh
_setArgs "$@"

# Call build function (uses BUILD_TYPE and all other config from _setArgs)
build

# Validate required configuration
if [ -z "$BENCHMARK_BASE_DIR" ]; then
    echo -e "${RED}Error: BENCHMARK_BASE_DIR must be set${NORMAL_OUTPUT}"
    exit 1
fi

if [ -z "$BENCHMARK_CSV" ] || [ -z "$BENCHMARK_DECLARATION" ]; then
    echo -e "${RED}Error: BENCHMARK_CSV and BENCHMARK_DECLARATION must be set${NORMAL_OUTPUT}"
    exit 1
fi

# Set defaults
BENCHMARK_FILE="${BENCHMARK_FILE:-$BENCHMARK_BASE_DIR/benchmark_${BUILD_TYPE}.csv}"
BENCHMARK_REPEATS="${BENCHMARK_REPEATS:-3}"
BENCHMARK_EXECUTABLE="${BENCHMARK_EXECUTABLE:-build/${BUILD_TYPE}/offline}"

# Handle compressed CSV if specified
if [ -n "$BENCHMARK_COMPRESSED_CSV" ] && [ ! -f "$BENCHMARK_BASE_DIR/$BENCHMARK_CSV" ]; then
    echo "Extracting $BENCHMARK_COMPRESSED_CSV..."
    tar -xf "$BENCHMARK_BASE_DIR/$BENCHMARK_COMPRESSED_CSV" --directory "$BENCHMARK_BASE_DIR"
fi

# Handle compressed expected_results if specified
if [ -n "$BENCHMARK_COMPRESSED_RESULTS" ] && [ ! -d "$BENCHMARK_BASE_DIR/expected_results" ]; then
    echo "Extracting expected results..."
    tar -xf "$BENCHMARK_BASE_DIR/$BENCHMARK_COMPRESSED_RESULTS" --directory "$BENCHMARK_BASE_DIR"
fi

# Find all queries
queries=$(find "$BENCHMARK_BASE_DIR/queries" -type f | sort -V)

# Initialize benchmark CSV with header
echo "query,time" > "$BENCHMARK_FILE"

temp_time_file=$(mktemp)
trap 'rm -f -- "$temp_time_file"' EXIT

# Benchmark each query
for query in $queries; do
    query_file=$(basename "$query")
    echo -e "\n${GREEN}Benchmarking ${query_file}${NORMAL_OUTPUT}"

    total_time=0

    # Run query multiple times
    for i in $(seq 1 $BENCHMARK_REPEATS); do
        echo "  Run $i of $BENCHMARK_REPEATS..."

        # Time the query execution
        /usr/bin/time -f "%e" -o "$temp_time_file" \
            scripts/run_single_query_and_check.sh \
            "$BENCHMARK_EXECUTABLE" \
            "$query" \
            "$BENCHMARK_BASE_DIR/$BENCHMARK_DECLARATION" \
            "$BENCHMARK_BASE_DIR/$BENCHMARK_CSV" \
            "$BENCHMARK_BASE_DIR/expected_results/$query_file" \
            $BENCHMARK_EXTRA_ARGS

        result=$?

        if [ $result -ne 0 ]; then
            echo -e "${RED}Query $query_file failed on run $i${NORMAL_OUTPUT}"
            rm -f -- "$temp_time_file"
            [ "$BENCHMARK_CLEANUP_CSV" = "true" ] && rm -f "$BENCHMARK_BASE_DIR/$BENCHMARK_CSV"
            [ -n "$BENCHMARK_COMPRESSED_RESULTS" ] && rm -rf "$BENCHMARK_BASE_DIR/expected_results"
            exit 1
        fi

        time_taken=$(cat "$temp_time_file")
        total_time=$(echo "$total_time + $time_taken" | bc -l)
    done

    # Calculate average and write to CSV immediately
    avg_time=$(echo "$total_time / $BENCHMARK_REPEATS" | bc -l)
    echo "$query_file,$avg_time" >> "$BENCHMARK_FILE"
    echo -e "${GREEN}  Average time: ${avg_time}s${NORMAL_OUTPUT}"
done

rm -f -- "$temp_time_file"

# Cleanup if requested
[ "$BENCHMARK_CLEANUP_CSV" = "true" ] && rm -f "$BENCHMARK_BASE_DIR/$BENCHMARK_CSV"
[ -n "$BENCHMARK_COMPRESSED_RESULTS" ] && rm -rf "$BENCHMARK_BASE_DIR/expected_results"

echo -e "\n${GREEN}=== Benchmark Complete ===${NORMAL_OUTPUT}"
echo "Results saved to $BENCHMARK_FILE"
