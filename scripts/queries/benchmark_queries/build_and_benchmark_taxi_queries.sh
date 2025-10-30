#!/bin/bash

# Work at the root directory
cd "$(dirname "$0")"
cd ../../..

export BENCHMARK_BASE_DIR="src/targets/experiments/taxis"
export BENCHMARK_CSV="taxi_data.csv"
export BENCHMARK_DECLARATION="declaration.core"
export BENCHMARK_COMPRESSED_CSV="taxi_data.tar.xz"
export BENCHMARK_CLEANUP_CSV=true
export BENCHMARK_COMPRESSED_RESULTS="expected_results.tar.xz"

exec scripts/queries/benchmark_queries/benchmark_runner.sh "$@"
