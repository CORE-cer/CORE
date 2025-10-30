#!/bin/bash

# Work at the root directory
cd "$(dirname "$0")" || exit
cd ../../.. || exit

export BENCHMARK_BASE_DIR="src/targets/experiments/unordered_stocks"
export BENCHMARK_CSV="stock_data.csv"
export BENCHMARK_DECLARATION="declaration.core"
export BENCHMARK_EXTRA_ARGS="-o $BENCHMARK_BASE_DIR/quarantine_declaration.core"
export BENCHMARK_COMPRESSED_RESULTS="expected_results.tar.xz"

exec scripts/queries/benchmark_queries/benchmark_runner.sh "$@"
